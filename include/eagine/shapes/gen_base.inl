/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/assert.hpp>
#include <eagine/math/functions.hpp>
#include <eagine/math/intersection.hpp>
#include <eagine/memory/span_algo.hpp>
#include <array>
#include <limits>
#include <vector>

#include <iostream>
namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
// generator
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto generator::bounding_sphere() -> math::sphere<float, true> {
    std::array<float, 3> min{
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::max(),
      std::numeric_limits<float>::max()};

    std::array<float, 3> max{
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest()};

    const auto attrib = vertex_attrib_kind::position;
    const auto n = vertex_count();
    const auto m = values_per_vertex(attrib);

    std::vector<float> temp(std_size(n * m));
    auto pos = cover(temp);
    attrib_values(attrib, pos);

    for(const auto v : integer_range(n)) {
        for(const auto c : integer_range(m)) {
            const auto k = std_size(c);

            min[k] = eagine::math::minimum(min[k], pos[v * m + c]);
            max[k] = eagine::math::maximum(max[k], pos[v * m + c]);
        }
    }

    math::tvec<float, 3, true> center{
      (min[0] + max[0]) * 0.5F,
      (min[1] + max[1]) * 0.5F,
      (min[2] + max[2]) * 0.5F};

    float radius{0.F};
    for(const auto c : integer_range(m)) {
        const auto k = std_size(c);
        const auto q = (max[k] - min[k]) * 0.5F;
        radius += q * q;
    }
    radius = std::sqrt(radius);

    return {center, radius};
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator::ray_intersections(
  drawing_variant var,
  span<const math::line<float, true>> rays,
  span<optionally_valid<float>> intersections) {

    EAGINE_ASSERT(intersections.size() >= rays.size());

    std::vector<draw_operation> ops(std_size(operation_count(var)));
    instructions(var, cover(ops));

    std::vector<std::uint32_t> idx(std_size(index_count(var)));
    indices(var, cover(idx));

    const auto pvak = vertex_attrib_kind::position;
    const auto vpv = values_per_vertex(pvak);

    std::vector<float> pos(std_size(vertex_count() * vpv));
    attrib_values(pvak, cover(pos));

    std::vector<std::size_t> ray_idx;

    const auto bs = bounding_sphere();

    for(const auto i : integer_range(rays.size())) {
        const auto nparam = math::nearest_ray_param(
          math::line_sphere_intersection_params(rays[i], bs));
        if(nparam >= 0.F) {
            ray_idx.push_back(std_size(i));
        }
    }

    auto intersect = [&ray_idx, &rays, &intersections](
                       const auto& fce, bool cw) {
        for(const auto i : ray_idx) {
            const auto& ray = rays[i];
            const auto nparam =
              math::line_triangle_intersection_param(ray, fce);

            if(nparam > 0.0001F) {
                const auto fnml = fce.normal(cw);
                if(dot(ray.direction(), fnml) < 0.F) {
                    auto& oparam = intersections[i];
                    if(!oparam || bool(nparam < oparam)) {
                        oparam = nparam;
                    }
                }
            }
        }
    };

    auto coord = [&pos, &idx, vpv](auto vx, auto cr, bool idxd) {
        if(idxd) {
            return pos[std_size(span_size(idx[std_size(vx)]) * vpv + cr)];
        } else {
            return pos[std_size(vx * vpv + cr)];
        }
    };

    for(const auto& op : ops) {
        const bool indexed = op.idx_type != index_data_type::none;

        if(op.mode == primitive_type::triangles) {
            std::array<std::array<float, 4>, 3> tri{};
            span_size_t t = 0;
            for(const auto v : integer_range(op.count)) {
                const auto w = v + op.first;
                for(const auto c : integer_range(3)) {
                    tri[std_size(t)][std_size(c)] = coord(w, c, indexed);
                }
                if(++t >= 3) {
                    t = 0;
                    math::triangle<float, true> face{
                      {tri[0][0], tri[0][1], tri[0][2]},
                      {tri[1][0], tri[1][1], tri[1][2]},
                      {tri[2][0], tri[2][1], tri[2][2]}};
                    intersect(face, op.cw_face_winding);
                }
            }
        } else if(op.mode == primitive_type::triangle_strip) {
            for(const auto v : integer_range(2, op.count)) {
                span_size_t w = v + op.first;
                span_size_t o0 = -2, o1 = -1, o2 = 0;
                if(v % 2 != 0) {
                    o1 = 0;
                    o2 = -1;
                }
                math::triangle<float, true> face{
                  {coord(w + o0, 0, indexed),
                   coord(w + o0, 1, indexed),
                   coord(w + o0, 2, indexed)},
                  {coord(w + o1, 0, indexed),
                   coord(w + o1, 1, indexed),
                   coord(w + o1, 2, indexed)},
                  {coord(w + o2, 0, indexed),
                   coord(w + o2, 1, indexed),
                   coord(w + o2, 2, indexed)}};
                intersect(face, op.cw_face_winding);
            }
        }
    }
}
//------------------------------------------------------------------------------
// generator_base
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC auto generator_base::index_type(drawing_variant)
  -> index_data_type {
    return index_data_type::none;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto generator_base::index_count(drawing_variant) -> span_size_t {
    return 0;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(drawing_variant, span<std::uint8_t>) {
    EAGINE_UNREACHABLE("Invalid function called for this index data type");
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(drawing_variant var, span<std::uint16_t> dest) {
    if(index_type(var) == index_data_type::unsigned_8) {
        std::vector<std::uint8_t> tmp(std_size(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(drawing_variant var, span<std::uint32_t> dest) {
    const auto ity = index_type(var);
    if(ity == index_data_type::unsigned_8) {
        std::vector<std::uint8_t> tmp(std_size(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    } else if(ity == index_data_type::unsigned_16) {
        std::vector<std::uint16_t> tmp(std_size(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    }
}
//------------------------------------------------------------------------------
// centered_unit_shape_generator_base
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void centered_unit_shape_generator_base::attrib_values(
  vertex_attrib_variant vav,
  span<float> dest) {
    if(vav.attribute() == vertex_attrib_kind::box_coord) {
        this->attrib_values({vertex_attrib_kind::position, vav}, dest);
        for(float& x : dest) {
            x += 0.5F;
        }
    } else if(vav == vertex_attrib_kind::pivot) {
        fill(head(dest, this->vertex_count() * 3), 0.F);
    } else if(vav == vertex_attrib_kind::pivot_pivot) {
        fill(head(dest, this->vertex_count() * 3), 0.F);
    } else if(vav == vertex_attrib_kind::vertex_pivot) {
        fill(head(dest, this->vertex_count() * 3), 0.F);
    } else {
        generator_base::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine
