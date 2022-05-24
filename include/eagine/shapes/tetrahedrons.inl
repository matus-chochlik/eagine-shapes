/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/assert.hpp>
#include <eagine/math/constants.hpp>

#ifdef __clang__
EAGINE_DIAG_PUSH()
EAGINE_DIAG_OFF(double-promotion)
#endif

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::box_coord | vertex_attrib_kind::vertex_coord;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::_shared_attrs() noexcept
  -> vertex_attrib_kinds {
    return vertex_attrib_kind::box_coord | vertex_attrib_kind::vertex_coord;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::_only_shared_attribs() noexcept -> bool {
    return !(attrib_kinds() & ~_shared_attrs());
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
marching_tetrahedrons_gen::marching_tetrahedrons_gen(
  const vertex_attrib_kinds attr_kinds) noexcept
  : _base(attr_kinds & _attr_mask(), generator_capability::indexed_drawing) {}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::vertex_count() -> span_size_t {
    return 8;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::coords(span<std::int16_t> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    // A
    dest[k++] = 0;
    dest[k++] = 0;
    dest[k++] = 0;
    // B
    dest[k++] = 1;
    dest[k++] = 0;
    dest[k++] = 0;
    // C
    dest[k++] = 0;
    dest[k++] = 1;
    dest[k++] = 0;
    // D
    dest[k++] = 1;
    dest[k++] = 1;
    dest[k++] = 0;
    // E
    dest[k++] = 0;
    dest[k++] = 0;
    dest[k++] = 1;
    // F
    dest[k++] = 1;
    dest[k++] = 0;
    dest[k++] = 1;
    // G
    dest[k++] = 0;
    dest[k++] = 1;
    dest[k++] = 1;
    // H
    dest[k++] = 1;
    dest[k++] = 1;
    dest[k++] = 1;

    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    switch(vav.attribute()) {
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::vertex_coord:
            return attrib_data_type::int_16;
        default:
            break;
    }
    return _base::attrib_type(vav);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::is_attrib_integral(
  const vertex_attrib_variant vav) -> bool {
    switch(vav.attribute()) {
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::vertex_coord:
            return true;
        default:
            break;
    }
    return _base::is_attrib_integral(vav);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::box_coord:
            coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::index_type(const drawing_variant)
  -> index_data_type {
    return index_data_type::unsigned_8;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::index_count(const drawing_variant)
  -> span_size_t {
    return 24;
}
//------------------------------------------------------------------------------
template <typename T>
void marching_tetrahedrons_gen::_indices(
  [[maybe_unused]] const drawing_variant var,
  span<T> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= index_count(var));

    span_size_t k = 0;

    const unsigned A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7;

    // 0
    dest[k++] = T(A);
    dest[k++] = T(B);
    dest[k++] = T(D);
    dest[k++] = T(H);
    // 1
    dest[k++] = T(B);
    dest[k++] = T(A);
    dest[k++] = T(F);
    dest[k++] = T(H);
    // 2
    dest[k++] = T(E);
    dest[k++] = T(A);
    dest[k++] = T(G);
    dest[k++] = T(H);
    // 3
    dest[k++] = T(E);
    dest[k++] = T(F);
    dest[k++] = T(A);
    dest[k++] = T(H);
    // 4
    dest[k++] = T(A);
    dest[k++] = T(C);
    dest[k++] = T(G);
    dest[k++] = T(H);
    // 5
    dest[k++] = T(C);
    dest[k++] = T(A);
    dest[k++] = T(D);
    dest[k++] = T(H);

    EAGINE_ASSERT(k == index_count(var));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::operation_count(const drawing_variant)
  -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void marching_tetrahedrons_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    EAGINE_ASSERT(ops.size() >= operation_count(var));

    draw_operation& op = ops[0];
    op.mode = primitive_type::tetrahedrons;
    op.idx_type = index_data_type::unsigned_8;
    op.first = 0;
    op.count = index_count(var);
    op.primitive_restart = false;
    op.cw_face_winding = false;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto marching_tetrahedrons_gen::bounding_sphere() -> math::sphere<float, true> {
    return {{0.0F}, 0.5F};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#ifdef __clang__
EAGINE_DIAG_POP()
#endif
