/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/assert.hpp>

#ifdef __clang__
EAGINE_DIAG_PUSH()
EAGINE_DIAG_OFF(double-promotion)
#endif

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_plane_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::wrap_coord | vertex_attrib_kind::face_coord |
           vertex_attrib_kind::box_coord | vertex_attrib_kind::vertex_coord;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
unit_plane_gen::unit_plane_gen(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) noexcept
  : _base(
      attr_kinds & _attr_mask(),
      generator_capability::primitive_restart |
        generator_capability::element_strips |
        generator_capability::indexed_drawing)
  , _width{extract(width)}
  , _height{extract(height)} {}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_plane_gen::vertex_count() -> span_size_t {
    return span_size((_width + 1) * (_height + 1));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::positions(span<float> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

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

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::normals(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::normal));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 0.F;
        dest[k++] = 1.F;
        dest[k++] = 0.F;
    }

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::tangents(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::tangent));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 1.F;
        dest[k++] = 0.F;
        dest[k++] = 0.F;
    }

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::bitangents(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::bitangent));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(vertex_count())) {
        dest[k++] = 0.F;
        dest[k++] = 0.F;
        dest[k++] = -1.F;
    }

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::box_coords(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::box_coord));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 2);

    span_size_t k = 0;

    const double dx{1.0 / _width};
    const double dy{1.0 / _height};
    for(int y = 0; y <= _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = float(0.0 + x * dx);
            dest[k++] = float(1.0 - y * dy);
        }
    }

    EAGINE_ASSERT(k == vertex_count() * 2);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::vertex_coords(span<std::int32_t> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::vertex_coord));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    for(int y = 0; y <= _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = limit_cast<std::int32_t>(x);
            dest[k++] = limit_cast<std::int32_t>(y);
            dest[k++] = 0;
        }
    }

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
auto unit_plane_gen::index_type(const drawing_variant) -> index_data_type {
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_plane_gen::index_count(const drawing_variant) -> span_size_t {
    return span_size(
      (2 * (_width + 1) + (primitive_restart() ? 1 : 0)) * _height);
}
//------------------------------------------------------------------------------
template <typename T>
void unit_plane_gen::_indices(const drawing_variant var, span<T> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= index_count(var));
    span_size_t k = 0;

    const bool pr{primitive_restart()};
    const auto pri{unsigned(vertex_count())};

    for(int y = 0; y < _height; ++y) {
        for(int x = 0; x <= _width; ++x) {
            dest[k++] = limit_cast<T>((y + 0) * (_width + 1) + x);
            dest[k++] = limit_cast<T>((y + 1) * (_width + 1) + x);
        }
        if(pr) {
            dest[k++] = pri;
        }
    }

    EAGINE_ASSERT(k == index_count(var));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto unit_plane_gen::operation_count(const drawing_variant) -> span_size_t {
    if(primitive_restart()) {
        return 1;
    }
    return span_size(_height);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void unit_plane_gen::instructions(
  [[maybe_unused]] const drawing_variant var,
  span<draw_operation> ops) {
    EAGINE_ASSERT(ops.size() >= operation_count(var));

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
EAGINE_LIB_FUNC
auto unit_plane_gen::bounding_sphere() -> math::sphere<float, true> {
    using std::sqrt;
    return {{0.0F}, float(sqrt(2.F))};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#ifdef __clang__
EAGINE_DIAG_POP()
#endif
