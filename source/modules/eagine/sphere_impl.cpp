/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import std;
import eagine.core;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class unit_sphere_gen : public generator_base {
public:
    unit_sphere_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_greater_than<int, 2>& rings,
      const valid_if_greater_than<int, 3>& sections) noexcept;

    unit_sphere_gen(const vertex_attrib_kinds attr_kinds) noexcept
      : unit_sphere_gen(attr_kinds, 12, 18) {}

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float> override;

    void ray_intersections(
      generator& gen,
      const drawing_variant,
      const span<const math::line<float>> rays,
      span<optionally_valid<float>> intersections) override;

private:
    using _base = generator_base;

    int _rings;
    int _sections;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
auto unit_sphere_from(
  const vertex_attrib_kinds attr_kinds,
  const url& locator,
  main_ctx&) -> shared_holder<generator> {
    if(locator.has_path("/unit_sphere")) {
        const auto rings{locator.query().arg_value_as(
          "rings", std::type_identity<valid_if_greater_than<int, 2>>{})};
        const auto sections{locator.query().arg_value_as(
          "sections", std::type_identity<valid_if_greater_than<int, 3>>{})};
        return unit_sphere(
          attr_kinds, rings.value_or(18), sections.value_or(36));
    }
    return {};
}
//------------------------------------------------------------------------------
auto unit_sphere(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections) -> shared_holder<generator> {
    return {hold<unit_sphere_gen>, attr_kinds, rings, sections};
}
//------------------------------------------------------------------------------
auto unit_sphere_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::box_coord | vertex_attrib_kind::wrap_coord;
}
//------------------------------------------------------------------------------
unit_sphere_gen::unit_sphere_gen(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections) noexcept
  : _base(
      attr_kinds & _attr_mask(),
      generator_capability::indexed_drawing |
        generator_capability::primitive_restart |
        generator_capability::element_strips)
  , _rings{rings.value()}
  , _sections{sections.value()} {}
//------------------------------------------------------------------------------
auto unit_sphere_gen::vertex_count() -> span_size_t {
    return (_rings + 1) * (_sections + 1);
}
//------------------------------------------------------------------------------
void unit_sphere_gen::positions(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::position));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto s_step = math::tau / _sections;
    const auto r_step = math::pi / _rings;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(_rings + 1)) {
            const auto r_lat = std::cos(r * r_step);
            const auto r_rad = std::sin(r * r_step);

            dest[k++] = float(0.5F * r_rad * std::cos(s * s_step));
            dest[k++] = float(0.5F * r_lat);
            dest[k++] = float(0.5F * r_rad * -std::sin(s * s_step));
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto s_step = math::tau / _sections;
    const auto r_step = math::pi / _rings;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(_rings + 1)) {
            const auto r_lat = std::cos(r * r_step);
            const auto r_rad = std::sin(r * r_step);

            dest[k++] = float(r_rad * std::cos(s * s_step));
            dest[k++] = float(r_lat);
            dest[k++] = float(r_rad * -std::sin(s * s_step));
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::tangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::tangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto s_step = math::tau / _sections;

    for(const auto s : integer_range(_sections + 1)) {
        auto x = -std::sin(s * s_step);
        auto z = -std::cos(s * s_step);

        for([[maybe_unused]] const auto r : integer_range(_rings + 1)) {
            dest[k++] = float(x);
            dest[k++] = float(0);
            dest[k++] = float(z);
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::bitangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::bitangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto s_step = math::tau / _sections;
    const auto r_step = math::pi / _rings;
    const auto ty = 0;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(_rings + 1)) {
            const auto r_rad = std::sin(r * r_step);
            const auto tx = -std::sin(s * s_step);
            const auto tz = -std::cos(s * s_step);
            const auto nx = -r_rad * tz;
            const auto ny = std::cos(r * r_step);
            const auto nz = r_rad * tx;

            dest[k++] = float(ny * tz - nz * ty);
            dest[k++] = float(nz * tx - nx * tz);
            dest[k++] = float(nx * ty - ny * tx);
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::wrap_coords(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::wrap_coord));
    assert(dest.size() >= vertex_count() * 2);

    span_size_t k = 0;

    const auto s_step = 1.0 / _sections;
    const auto r_step = 1.0 / _rings;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(_rings + 1)) {
            dest[k++] = float(s * s_step);
            dest[k++] = float(r * r_step);
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::position:
            positions(dest);
            break;
        case vertex_attrib_kind::normal:
            normals(dest);
            break;
        case vertex_attrib_kind::tangent:
            tangents(dest);
            break;
        case vertex_attrib_kind::bitangent:
            bitangents(dest);
            break;
        case vertex_attrib_kind::wrap_coord:
            wrap_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
auto unit_sphere_gen::index_type(const drawing_variant v) -> index_data_type {
    if(index_count(v) < span_size(std::numeric_limits<std::uint8_t>::max())) {
        return index_data_type::unsigned_8;
    }
    if(index_count(v) < span_size(std::numeric_limits<std::uint16_t>::max())) {
        return index_data_type::unsigned_16;
    }
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
auto unit_sphere_gen::index_count(const drawing_variant) -> span_size_t {
    return (_sections + 1) * ((_rings + 1) * 2 + (primitive_restart() ? 1 : 0));
}
//------------------------------------------------------------------------------
template <typename T>
void unit_sphere_gen::_indices(
  [[maybe_unused]] const drawing_variant var,
  span<T> dest) noexcept {
    assert(dest.size() >= index_count(var));

    const auto pri = limit_cast<T>(vertex_count());
    span_size_t k = 0;
    span_size_t step = _rings + 1;

    for(const auto s : integer_range(_sections + 1)) {
        for(const auto r : integer_range(step)) {
            dest[k++] = limit_cast<T>((s + 0) * step + r);
            dest[k++] = limit_cast<T>((s + 1) * step + r);
        }

        if(primitive_restart()) {
            dest[k++] = pri;
        }
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_sphere_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_sphere_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto unit_sphere_gen::operation_count(const drawing_variant) -> span_size_t {
    if(primitive_restart()) {
        return 1;
    } else {
        return _sections;
    }
}
//------------------------------------------------------------------------------
void unit_sphere_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    assert(ops.size() >= operation_count(var));

    if(primitive_restart()) {
        draw_operation& op = ops[0];
        op.mode = primitive_type::triangle_strip;
        op.idx_type = index_type(var);
        op.first = 0;
        op.count = index_count(var);
        op.primitive_restart_index = unsigned(vertex_count());
        op.primitive_restart = true;
        op.cw_face_winding = true;
    } else {
        span_size_t step = 2 * (_rings + 1);
        for(const auto s : integer_range(_sections)) {
            draw_operation& op = ops[s];
            op.mode = primitive_type::triangle_strip;
            op.idx_type = index_type(var);
            op.first = s * step;
            op.count = step;
            op.primitive_restart = false;
            op.cw_face_winding = true;
        }
    }
}
//------------------------------------------------------------------------------
auto unit_sphere_gen::bounding_sphere() -> math::sphere<float> {
    return {{0.F, 0.F, 0.F}, 0.5F};
}
//------------------------------------------------------------------------------
void unit_sphere_gen::ray_intersections(
  generator& gen,
  const drawing_variant,
  const span<const math::line<float>> rays,
  span<optionally_valid<float>> intersections) {

    assert(intersections.size() >= rays.size());

    const auto bs = gen.bounding_sphere();

    for(const auto i : index_range(intersections)) {
        const auto& ray = rays[i];
        const auto nparam = math::nearest_ray_param(
          math::line_sphere_intersection_params(ray, bs));
        if(nparam > 0.0001F) {
            auto& oparam = intersections[i];
            if(not oparam or bool(nparam < oparam)) {
                oparam = nparam;
            }
        }
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

