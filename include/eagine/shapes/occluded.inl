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
void occluded_gen::attrib_values(vertex_attrib_variant vav, span<float> dest) {

    if(vav == vertex_attrib_kind::occlusion) {
        std::random_device rd;
        std::mt19937 rho_re(rd());
        std::mt19937 phi_re(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);

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

            std::vector<math::line<float, true>> rays(std_size(vc * ns));
            std::vector<float> weights(rays.size());

            for(const auto v : integer_range(vc)) {
                const auto k = std_size(v * 3);
                const math::tvec<float, 3, true> pos{
                  positions[k + 0], positions[k + 1], positions[k + 2]};
                const math::tvec<float, 3, true> nml{
                  normals[k + 0], normals[k + 1], normals[k + 2]};

                rays[std_size(v * ns)] = math::line<float, true>{pos, nml};
                weights[std_size(v * ns)] = 1.F;

                for(const auto s : integer_range(1, ns)) {
                    using std::acos;

                    const math::unit_spherical_coordinate<float, true> usc{
                      turns_(float(dis(rho_re))),
                      radians_(float(acos(2 * dis(phi_re) - 1)))};

                    auto dir = math::to_cartesian(usc);
                    auto wght = dot(dir, nml);

                    if(wght < 0.F) {
                        dir = -dir;
                        wght = -wght;
                    }

                    const auto l = std_size(v * ns + s);

                    rays[l] = math::line<float, true>{pos, dir};
                    weights[l] = wght;
                }
            }
            std::vector<optionally_valid<float>> params(rays.size());

            delegated_gen::ray_intersections(view(rays), cover(params));

            for(const auto v : integer_range(vc)) {
                float occl = 0.F;
                float wght = 0.F;
                for(const auto s : integer_range(ns)) {
                    const auto l = std_size(v * ns + s);
                    if(params[l] > 0.0F) {
                        using std::exp;
                        occl +=
                          exp(1.F - params[l].value_anyway()) * weights[l];
                    }
                    wght += weights[l];
                }
                dest[v] = occl / wght;
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
