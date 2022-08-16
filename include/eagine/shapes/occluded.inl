/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/assert.hpp>
#include <eagine/math/coordinates.hpp>
#include <eagine/math/functions.hpp>
#include <eagine/memory/span_algo.hpp>
#include <eagine/progress/activity.hpp>
#include <eagine/shapes/cached.hpp>
#include <atomic>
#include <random>
#include <thread>
#include <vector>

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
occluded_gen::occluded_gen(
  std::shared_ptr<generator> gen,
  const span_size_t samples,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"OcclShpGen", parent}
  , delegated_gen{cache(std::move(gen), this->as_parent())}
  , _samples{samples} {
    delegated_gen::_add(vertex_attrib_kind::occlusion);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void occluded_gen::occlusions(
  const vertex_attrib_variant vav,
  span<float> dest) {
    const auto vc = delegated_gen::vertex_count();
    const auto pva = vertex_attrib_kind::position;
    const auto nva = vertex_attrib_kind::normal;
    const auto pvpv = delegated_gen::values_per_vertex({pva, vav});
    const auto nvpv = delegated_gen::values_per_vertex({nva, vav});
    const auto ns = _samples;

    if((pvpv == 3) && (nvpv == 3)) {
        EAGINE_ASSERT(
          dest.size() >= vc * delegated_gen::values_per_vertex(vav));

        std::vector<float> positions(std_size(vc * pvpv));
        std::vector<float> normals(std_size(vc * nvpv));

        delegated_gen::attrib_values({pva, vav}, cover(positions));
        delegated_gen::attrib_values({nva, vav}, cover(normals));

        std::atomic<span_size_t> vi{0};
        std::random_device rd;

        const auto make_raytracer = [&](auto progress_update) {
            return [this,
                    &dest,
                    &positions,
                    &normals,
                    &vi,
                    vc,
                    ns,
                    pvpv,
                    nvpv,
                    progress_update{std::move(progress_update)},
                    rho_rs{rd()},
                    phi_rs{rd()}]() {
                std::default_random_engine rho_re{rho_rs};
                std::default_random_engine phi_re{phi_rs};
                std::uniform_real_distribution<double> dis{0.0, 1.0};

                std::vector<math::line<float, true>> rays(std_size(ns));
                std::vector<float> weights(rays.size());
                std::vector<optionally_valid<float>> params(rays.size());

                const auto vertex_occlusion = [&](const auto v) -> float {
                    const auto pk = std_size(v * pvpv);
                    const auto nk = std_size(v * nvpv);
                    const math::tvec<float, 3, true> pos{
                      positions[pk + 0], positions[pk + 1], positions[pk + 2]};
                    const math::tvec<float, 3, true> nml{
                      normals[nk + 0], normals[nk + 1], normals[nk + 2]};

                    rays[0] = math::line<float, true>{pos, nml};
                    weights[0] = 1.F;

                    for(const auto s : integer_range(1, ns)) {
                        const math::unit_spherical_coordinate<float, true> usc{
                          turns_(float(dis(rho_re))),
                          radians_(float(std::acos(2 * dis(phi_re) - 1)))};

                        auto dir = math::to_cartesian(usc);
                        auto wght = dot(dir, nml);

                        if(wght < 0.F) {
                            dir = -dir;
                            wght = -wght;
                        }

                        rays[s] = math::line<float, true>{pos, dir};
                        weights[s] = wght;
                    }
                    fill(cover(params), optionally_valid<float>{});
                    delegated_gen::ray_intersections(
                      *this, 0, view(rays), cover(params));

                    float occl = 0.F;
                    float wght = 0.F;
                    for(const auto s : integer_range(ns)) {
                        if(params[s] > 0.0F) {
                            const auto aip = extract(params[s]);
                            occl += (std::exp(-0.25F * aip) +
                                     std::exp(-0.125F * aip) +
                                     std::exp(-0.03125F * aip) +
                                     std::exp(-0.0078125F * aip)) *
                                    0.25F * weights[s];
                        }
                        wght += weights[s];
                    }
                    return occl / wght;
                };

                while(true) {
                    const auto v = vi++;
                    if(v < vc) {
                        dest[v] = vertex_occlusion(v);
                        if(!progress_update(v)) [[unlikely]] {
                            break;
                        }
                    } else {
                        break;
                    }
                }
            };
        };

        const auto worker_count = std::thread::hardware_concurrency();
        std::vector<std::thread> workers;
        workers.reserve(worker_count);
        for([[maybe_unused]] const auto t : integer_range(worker_count)) {
            workers.emplace_back(
              make_raytracer([](const auto) { return true; }));
        }

        make_raytracer(
          [raytracing{progress().activity("ray-tracing occlusions", vc)}](
            const auto v) { return raytracing.update_progress(v); })();
        vi = vc;

        for(auto& worker : workers) {
            worker.join();
        }
    } else {
        fill(dest, 0.F);
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void occluded_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    if(vav == vertex_attrib_kind::occlusion) {
        occlusions(vav, dest);
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
