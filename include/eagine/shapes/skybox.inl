/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/assert.hpp>
#include <eagine/math/tvec.hpp>
#include <eagine/memory/span_algo.hpp>

#ifdef __clang__
EAGINE_DIAG_PUSH()
EAGINE_DIAG_OFF(double-promotion)
#endif

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::pivot |
           vertex_attrib_kind::face_coord;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
skybox_gen::skybox_gen(const vertex_attrib_kinds attr_kinds) noexcept
  : _base(attr_kinds & _attr_mask(), generator_capability::indexed_drawing) {}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::vertex_count() -> span_size_t {
    return 8 + 6;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::positions(span<float> dest) noexcept {
    EAGINE_ASSERT(has(vertex_attrib_kind::face_coord));
    EAGINE_ASSERT(dest.size() >= vertex_count() * 3);

    /*
     *    (c)-----(d)    (Y)
     *    /|      /|      ^
     *   / |     / |      |
     * (g)-|---(h) |      |
     *  | (a)---|-(b)     O----> (X)
     *  | /     | /      /
     *  |/      |/      /
     * (e)-----(f)     L (Z)
     *
     *        (-)        (+)
     *    (c)----(g) (h)----(d)
     *     ^      |   ^      |
     *     |      |   |      |
     * (X) |      |   |      |
     *     |      |   |      |
     *    (a)--->(e) (f)--->(b)
     *
     *        (-)        (+)
     *    (e)----(f) (c)----(d)
     *     ^      |   ^      |
     *     |      |   |      |
     * (Y) |      |   |      |
     *     |      |   |      |
     *    (a)--->(b) (g)--->(h)
     *
     *        (-)        (+)
     *    (d)----(c) (g)----(h)
     *     ^      |   ^      |
     *     |      |   |      |
     * (Z) |      |   |      |
     *     |      |   |      |
     *    (b)--->(a) (e)--->(f)
     */

    span_size_t k = 0;
    // clang-format off
    dest[k++] = -1.F; dest[k++] = -1.F; dest[k++] = -1.F; // a
    dest[k++] =  1.F; dest[k++] = -1.F; dest[k++] = -1.F; // b
    dest[k++] = -1.F; dest[k++] =  1.F; dest[k++] = -1.F; // c
    dest[k++] =  1.F; dest[k++] =  1.F; dest[k++] = -1.F; // d
    dest[k++] = -1.F; dest[k++] = -1.F; dest[k++] =  1.F; // e
    dest[k++] =  1.F; dest[k++] = -1.F; dest[k++] =  1.F; // f
    dest[k++] = -1.F; dest[k++] =  1.F; dest[k++] =  1.F; // g
    dest[k++] =  1.F; dest[k++] =  1.F; dest[k++] =  1.F; // h

    const float rt3{std::sqrt(3.F)};
    dest[k++] = -rt3; dest[k++] =  0.F; dest[k++] =  0.F; // -x
    dest[k++] =  rt3; dest[k++] =  0.F; dest[k++] =  0.F; // +x
    dest[k++] =  0.F; dest[k++] = -rt3; dest[k++] =  0.F; // -y
    dest[k++] =  0.F; dest[k++] =  rt3; dest[k++] =  0.F; // +y
    dest[k++] =  0.F; dest[k++] =  0.F; dest[k++] = -rt3; // -y
    dest[k++] =  0.F; dest[k++] =  0.F; dest[k++] =  rt3; // +y
    // clang-format on
    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::face_coords(span<float> dest) noexcept {
    span_size_t k = 0;
    // clang-format off
    dest[k++] = -1.F; dest[k++] = -1.F; dest[k++] =  1.F; // a
    dest[k++] =  1.F; dest[k++] = -1.F; dest[k++] =  1.F; // b
    dest[k++] = -1.F; dest[k++] =  1.F; dest[k++] =  1.F; // c
    dest[k++] =  1.F; dest[k++] =  1.F; dest[k++] =  1.F; // d
    dest[k++] = -1.F; dest[k++] = -1.F; dest[k++] = -1.F; // e
    dest[k++] =  1.F; dest[k++] = -1.F; dest[k++] = -1.F; // f
    dest[k++] = -1.F; dest[k++] =  1.F; dest[k++] = -1.F; // g
    dest[k++] =  1.F; dest[k++] =  1.F; dest[k++] = -1.F; // h

    dest[k++] = -1.F; dest[k++] =  0.F; dest[k++] =  0.F; // -x
    dest[k++] =  1.F; dest[k++] =  0.F; dest[k++] =  0.F; // +x
    dest[k++] =  0.F; dest[k++] = -1.F; dest[k++] =  0.F; // -y
    dest[k++] =  0.F; dest[k++] =  1.F; dest[k++] =  0.F; // +y
    dest[k++] =  0.F; dest[k++] =  0.F; dest[k++] =  1.F; // -z
    dest[k++] =  0.F; dest[k++] =  0.F; dest[k++] = -1.F; // +z
    // clang-format on
    EAGINE_ASSERT(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::position:
            positions(dest);
            break;
        case vertex_attrib_kind::face_coord:
            face_coords(dest);
            break;
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangential:
        case vertex_attrib_kind::bitangential:
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::vertex_pivot:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::color:
        case vertex_attrib_kind::occlusion:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::draw_variant_count() -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::index_type(const drawing_variant) -> index_data_type {
    return index_data_type::unsigned_8;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::index_count(const drawing_variant) -> span_size_t {
    return 6 * 4 * 3;
}
//------------------------------------------------------------------------------
template <typename T>
inline void skybox_gen::_indices(
  [[maybe_unused]] const drawing_variant var,
  span<T> dest) noexcept {
    EAGINE_ASSERT(dest.size() >= index_count(var));

    span_size_t k = 0;

    const auto _a = T(0);
    const auto _b = T(1);
    const auto _c = T(2);
    const auto _d = T(3);
    const auto _e = T(4);
    const auto _f = T(5);
    const auto _g = T(6);
    const auto _h = T(7);

    const auto xn = T(8);
    const auto xp = T(9);
    const auto yn = T(10);
    const auto yp = T(11);
    const auto zn = T(12);
    const auto zp = T(13);

    // clang-format off
    // -x
    dest[k++] = xn; dest[k++] = _a; dest[k++] = _e;
    dest[k++] = xn; dest[k++] = _e; dest[k++] = _g;
    dest[k++] = xn; dest[k++] = _g; dest[k++] = _c;
    dest[k++] = xn; dest[k++] = _c; dest[k++] = _a;
    // +x
    dest[k++] = xp; dest[k++] = _f; dest[k++] = _b;
    dest[k++] = xp; dest[k++] = _b; dest[k++] = _d;
    dest[k++] = xp; dest[k++] = _d; dest[k++] = _h;
    dest[k++] = xp; dest[k++] = _h; dest[k++] = _f;
    // -y
    dest[k++] = yn; dest[k++] = _a; dest[k++] = _b;
    dest[k++] = yn; dest[k++] = _b; dest[k++] = _f;
    dest[k++] = yn; dest[k++] = _f; dest[k++] = _e;
    dest[k++] = yn; dest[k++] = _e; dest[k++] = _a;
    // +y
    dest[k++] = yp; dest[k++] = _g; dest[k++] = _h;
    dest[k++] = yp; dest[k++] = _h; dest[k++] = _d;
    dest[k++] = yp; dest[k++] = _d; dest[k++] = _c;
    dest[k++] = yp; dest[k++] = _c; dest[k++] = _g;
    // -z
    dest[k++] = zn; dest[k++] = _b; dest[k++] = _a;
    dest[k++] = zn; dest[k++] = _a; dest[k++] = _c;
    dest[k++] = zn; dest[k++] = _c; dest[k++] = _d;
    dest[k++] = zn; dest[k++] = _d; dest[k++] = _b;
    // +z
    dest[k++] = zp; dest[k++] = _e; dest[k++] = _f;
    dest[k++] = zp; dest[k++] = _f; dest[k++] = _h;
    dest[k++] = zp; dest[k++] = _h; dest[k++] = _g;
    dest[k++] = zp; dest[k++] = _g; dest[k++] = _e;
    // clang-format on

    EAGINE_ASSERT(k == index_count(var));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::operation_count(const drawing_variant) -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void skybox_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    EAGINE_ASSERT(ops.size() >= operation_count(var));

    draw_operation& op = ops[0];
    op.mode = primitive_type::triangles;
    op.idx_type = index_type(var);
    op.first = 0;
    op.count = index_count(var);
    op.primitive_restart = false;
    op.cw_face_winding = true;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto skybox_gen::bounding_sphere() -> math::sphere<float, true> {
    using std::sqrt;
    return {{0.0F}, float(sqrt(2.F))};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#ifdef __clang__
EAGINE_DIAG_POP()
#endif
