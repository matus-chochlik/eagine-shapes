/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import <cmath>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto unit_screen_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::wrap_coord | vertex_attrib_kind::face_coord |
           vertex_attrib_kind::box_coord;
}
//------------------------------------------------------------------------------
unit_screen_gen::unit_screen_gen(const vertex_attrib_kinds attr_kinds) noexcept
  : _base(attr_kinds & _attr_mask(), generator_capability::element_strips) {}
//------------------------------------------------------------------------------
auto unit_screen_gen::vertex_count() -> span_size_t {
    return 4;
}
//------------------------------------------------------------------------------
void unit_screen_gen::positions(span<float> dest) noexcept {
    assert(dest.size() >= vertex_count() * 3);

    /*
     * [-1, 1] [ 1, 1]
     *    (1)---(3)
     *     | \   |
     *     |  \  |
     *     |   \ |
     *    (0)---(2)
     * [-1,-1] [ 1,-1]
     */

    span_size_t k = 0;
    // (0)
    dest[k++] = -1.F;
    dest[k++] = -1.F;
    dest[k++] = 0.F;
    // (1)
    dest[k++] = -1.F;
    dest[k++] = 1.F;
    dest[k++] = 0.F;
    // (2)
    dest[k++] = 1.F;
    dest[k++] = -1.F;
    dest[k++] = 0.F;
    // (3)
    dest[k++] = 1.F;
    dest[k++] = 1.F;
    dest[k++] = 0.F;

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_screen_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(4)) {
        dest[k++] = 0.F;
        dest[k++] = 0.F;
        dest[k++] = 1.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_screen_gen::tangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::tangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(4)) {
        dest[k++] = 1.F;
        dest[k++] = 0.F;
        dest[k++] = 0.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_screen_gen::bitangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::bitangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    for([[maybe_unused]] const auto i : integer_range(4)) {
        dest[k++] = 0.F;
        dest[k++] = 1.F;
        dest[k++] = 0.F;
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_screen_gen::face_coords(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::face_coord));
    assert(dest.size() >= vertex_count() * 2);

    /*
     *  [0, 1]  [1, 1]
     *    (1)---(3)
     *     | \   |
     *     |  \  |
     *     |   \ |
     *    (0)---(2)
     *  [0, 0]  [1, 0]
     */

    span_size_t k = 0;
    // (0)
    dest[k++] = 0.F;
    dest[k++] = 0.F;
    // (1)
    dest[k++] = 0.F;
    dest[k++] = 1.F;
    // (2)
    dest[k++] = 1.F;
    dest[k++] = 0.F;
    // (3)
    dest[k++] = 1.F;
    dest[k++] = 1.F;

    assert(k == vertex_count() * 2);
}
//------------------------------------------------------------------------------
void unit_screen_gen::attrib_values(
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
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::wrap_coord:
            face_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
auto unit_screen_gen::operation_count(const drawing_variant) -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
void unit_screen_gen::instructions(
  [[maybe_unused]] const drawing_variant var,
  span<draw_operation> ops) {
    assert(ops.size() >= operation_count(var));

    draw_operation& op = ops[0];
    op.mode = primitive_type::triangle_strip;
    op.idx_type = index_data_type::none;
    op.first = 0;
    op.count = vertex_count();
    op.primitive_restart = false;
    op.cw_face_winding = true;
}
//------------------------------------------------------------------------------
auto unit_screen_gen::bounding_sphere() -> math::sphere<float, true> {
    using std::sqrt;
    return {{0.0F}, float(sqrt(2.F))};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

