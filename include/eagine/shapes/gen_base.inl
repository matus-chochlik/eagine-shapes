/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/assert.hpp>
#include <eagine/integer_range.hpp>
#include <eagine/math/functions.hpp>
#include <eagine/math/intersection.hpp>
#include <eagine/memory/span_algo.hpp>
#include <eagine/reflect/enumerators.hpp>
#include <array>
#include <limits>
#include <vector>

namespace eagine::shapes {
//------------------------------------------------------------------------------
// generator
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto generator::find_variant(
  const vertex_attrib_kind attrib,
  const string_view name) -> vertex_attrib_variant {
    const span_size_t n = attribute_variants(attrib);
    span_size_t index{-1};
    for(const auto i : integer_range(n)) {
        if(are_equal(name, variant_name({attrib, i}))) {
            index = i;
            break;
        }
    }
    return {attrib, index};
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto generator::find_variant(const string_view name) -> vertex_attrib_variant {
    for(const auto& info : enumerator_mapping(
          type_identity<vertex_attrib_kind>(), default_selector)) {
        if(auto found{find_variant(info.enumerator, name)}) {
            return found;
        }
    }
    return {};
}
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
void generator::for_each_triangle(
  generator& gen,
  const drawing_variant var,
  const callable_ref<void(const shape_face_info&)> callback) {

    std::vector<draw_operation> ops(std_size(gen.operation_count(var)));
    gen.instructions(var, cover(ops));

    std::vector<std::uint32_t> idx(std_size(gen.index_count(var)));
    gen.indices(var, cover(idx));

    const auto get_index = [&idx](auto vx, bool idxd) {
        if(idxd) {
            return span_size(idx[std_size(vx)]);
        } else {
            return span_size(vx);
        }
    };

    shape_face_info tri{};

    for(const auto& op : ops) {
        const bool indexed = op.idx_type != index_data_type::none;
        tri.cw_face_winding = op.cw_face_winding;

        if(op.mode == primitive_type::triangles) {
            span_size_t t = 0;
            for(const auto v : integer_range(op.count)) {
                const auto w = v + op.first;
                tri.indices[std_size(t)] = get_index(w, indexed);
                if(++t >= 3) {
                    t = 0;
                    callback(tri);
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
                tri.indices = {
                  {get_index(w + o0, indexed),
                   get_index(w + o1, indexed),
                   get_index(w + o2, indexed)}};
                callback(tri);
            }
        }
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator::random_surface_values(const random_attribute_values& values) {
    EAGINE_ASSERT(are_consistent(values));
    EAGINE_MAYBE_UNUSED(values);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator::ray_intersections(
  generator& gen,
  const drawing_variant var,
  const span<const math::line<float, true>> rays,
  span<optionally_valid<float>> intersections) {

    EAGINE_ASSERT(intersections.size() >= rays.size());

    const auto pvak = vertex_attrib_kind::position;
    const auto vpv = gen.values_per_vertex(pvak);

    std::vector<float> pos(std_size(gen.vertex_count() * vpv));
    gen.attrib_values(pvak, cover(pos));

    std::vector<std::size_t> ray_idx;

    const auto bs = gen.bounding_sphere();

    for(const auto i : integer_range(rays.size())) {
        const auto nparam = math::nearest_ray_param(
          math::line_sphere_intersection_params(rays[i], bs));
        if(nparam >= 0.F) {
            ray_idx.push_back(std_size(i));
        }
    }

    if(!ray_idx.empty()) {
        const auto intersect = [&ray_idx, &rays, &intersections](
                                 const auto& fce, const bool cw) {
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

        const auto find_intersections =
          [&intersect, &pos, vpv](const shape_face_info& info) {
              const math::triangle<float, true> face{
                {pos[info.indices[0] * vpv + 0],
                 pos[info.indices[0] * vpv + 1],
                 pos[info.indices[0] * vpv + 2]},
                {pos[info.indices[1] * vpv + 0],
                 pos[info.indices[1] * vpv + 1],
                 pos[info.indices[1] * vpv + 2]},
                {pos[info.indices[2] * vpv + 0],
                 pos[info.indices[2] * vpv + 1],
                 pos[info.indices[2] * vpv + 2]}};
              intersect(face, info.cw_face_winding);
          };

        gen.for_each_triangle(gen, var, {construct_from, find_intersections});
    }
}
//------------------------------------------------------------------------------
// generator_base
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC auto generator_base::index_type(const drawing_variant)
  -> index_data_type {
    return index_data_type::none;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto generator_base::index_count(const drawing_variant) -> span_size_t {
    return 0;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(const drawing_variant, span<std::uint8_t>) {
    EAGINE_UNREACHABLE("Invalid function called for this index data type");
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    if(index_type(var) == index_data_type::unsigned_8) {
        std::vector<std::uint8_t> tmp(std_size(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void generator_base::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
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
  const vertex_attrib_variant vav,
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
} // namespace eagine::shapes
