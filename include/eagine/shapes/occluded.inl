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
#include <random>
#include <vector>

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void occluded_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    if(vav == vertex_attrib_kind::occlusion) {
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

            std::random_device rd;
            std::default_random_engine rho_re(rd());
            std::default_random_engine phi_re(rd());
            std::uniform_real_distribution<double> dis(0.0, 1.0);

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
                delegated_gen::ray_intersections(view(rays), cover(params));

                float occl = 0.F;
                float wght = 0.F;
                for(const auto s : integer_range(ns)) {
                    if(params[s] > 0.0F) {
                        const auto aip = extract(params[s]) * 2.F;
                        occl += std::exp(-aip) * weights[s];
                    }
                    wght += weights[s];
                }
                return occl / wght;
            };

            for(const auto v : integer_range(vc)) {
                dest[v] = vertex_occlusion(v);
            }
        } else {
            fill(dest, 0.F);
        }
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine
