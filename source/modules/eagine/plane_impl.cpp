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
class unit_plane_gen : public generator_base {
public:
    unit_plane_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_positive<int>& width,
      const valid_if_positive<int>& height) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void box_coords(span<float> dest) noexcept;

    void vertex_coords(span<std::int32_t> dest) noexcept;

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override;

    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float> override;

private:
    using _base = generator_base;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    int _width;
    int _height;
};
//------------------------------------------------------------------------------
auto unit_plane_from(
  const vertex_attrib_kinds attr_kinds,
  const url& locator,
  main_ctx& ctx) -> shared_holder<generator> {
    if(locator.has_path("/unit_plane")) {
        using D = valid_if_positive<int>;
        auto width{locator.query().arg_value_as<D>("width")};
        auto height{locator.query().arg_value_as<D>("height")};
        if(width and height) {
            return unit_plane(attr_kinds, *width, *height);
        } else if(not width) {
            ctx.log()
              .error("missing or invalid width creating plane shape generator")
              .arg("URL", locator.str());
        } else if(not height) {
            ctx.log()
              .error("missing or invalid height creating plane shape generator")
              .arg("URL", locator.str());
        }
        return unit_plane(attr_kinds);
    }
    return {};
}
//------------------------------------------------------------------------------
auto unit_plane(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) -> shared_holder<generator> {
    return {hold<unit_plane_gen>, attr_kinds, width, height};
}
//------------------------------------------------------------------------------
auto unit_plane_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::wrap_coord | vertex_attrib_kind::face_coord |
           vertex_attrib_kind::box_coord | vertex_attrib_kind::vertex_coord;
}
//------------------------------------------------------------------------------
unit_plane_gen::unit_plane_gen(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) noexcept
  : _base(
      attr_kinds & _attr_mask(),
      generator_capability::primitive_restart |
        generator_capability::element_strips |
        generator_capability::indexed_drawing)
  , _width{*width}
  , _height{*height} {}
//------------------------------------------------------------------------------
auto unit_plane_gen::vertex_count() -> span_size_t {
    return span_size((_width + 1) * (_height + 1));
}
//------------------------------------------------------------------------------
void unit_plane_gen::positions(span<float> dest) noexcept {
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const double dx{2.0 / _width};
    const double dy{2.0 / _height};
    for(int y = 0; y <= _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = float(x * dx - 1.0);
            dest[k++] = 0.F;
            dest[k++] = float(1.0 - y * dy);
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_plane_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 0.F;
        dest[k++] = 1.F;
        dest[k++] = 0.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_plane_gen::tangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::tangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 1.F;
        dest[k++] = 0.F;
        dest[k++] = 0.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_plane_gen::bitangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::bitangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 0.F;
        dest[k++] = 0.F;
        dest[k++] = -1.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_plane_gen::box_coords(span<float> dest) noexcept {
    assert(
      has(vertex_attrib_kind::box_coord) or
      has(vertex_attrib_kind::wrap_coord));
    assert(dest.size() >= vertex_count() * 2);

    span_size_t k = 0;

    const double dx{1.0 / _width};
    const double dy{1.0 / _height};
    for(int y = 0; y <= _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = float(0.0 + x * dx);
            dest[k++] = float(1.0 - y * dy);
        }
    }

    assert(k == vertex_count() * 2);
}
//------------------------------------------------------------------------------
void unit_plane_gen::vertex_coords(span<std::int32_t> dest) noexcept {
    assert(has(vertex_attrib_kind::vertex_coord));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    for(int y = 0; y <= _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = limit_cast<std::int32_t>(x);
            dest[k++] = limit_cast<std::int32_t>(y);
            dest[k++] = 0;
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
auto unit_plane_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    switch(vav.attribute()) {
        case vertex_attrib_kind::vertex_coord:
            return attrib_data_type::int_32;
        default:
            break;
    }
    return _base::attrib_type(vav);
}
//------------------------------------------------------------------------------
void unit_plane_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::vertex_coord:
            vertex_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
void unit_plane_gen::attrib_values(
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
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::wrap_coord:
            box_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
auto unit_plane_gen::index_type(const drawing_variant) -> index_data_type {
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
auto unit_plane_gen::index_count(const drawing_variant) -> span_size_t {
    return span_size(
      (2 * (_width + 1) + (primitive_restart() ? 1 : 0)) * _height);
}
//------------------------------------------------------------------------------
template <typename T>
void unit_plane_gen::_indices(
  [[maybe_unused]] const drawing_variant var,
  span<T> dest) noexcept {
    assert(dest.size() >= index_count(var));
    span_size_t k = 0;

    const bool pr{primitive_restart()};
    const auto pri{unsigned(vertex_count())};

    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = limit_cast<T>((y + 0) * (_width + 1) + x);
            dest[k++] = limit_cast<T>((y + 1) * (_width + 1) + x);
        }
        if(pr) {
            dest[k++] = limit_cast<T>(pri);
        }
    }

    assert(k == index_count(var));
}
//------------------------------------------------------------------------------
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto unit_plane_gen::operation_count(const drawing_variant) -> span_size_t {
    if(primitive_restart()) {
        return 1;
    }
    return span_size(_height);
}
//------------------------------------------------------------------------------
void unit_plane_gen::instructions(
  [[maybe_unused]] const drawing_variant var,
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
        for(const auto y : integer_range(span_size(_height))) {
            draw_operation& op = ops[y];
            op.mode = primitive_type::triangle_strip;
            op.idx_type = index_type(var);
            op.first = 0;
            op.count = index_count(var);
            op.primitive_restart = false;
            op.cw_face_winding = true;
        }
    }
}
//------------------------------------------------------------------------------
auto unit_plane_gen::bounding_sphere() -> math::sphere<float> {
    using std::sqrt;
    return {{}, float(sqrt(2.F))};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

