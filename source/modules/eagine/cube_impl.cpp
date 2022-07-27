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
import <cstdint>;
import <cmath>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class unit_cube_gen : public generator_base {
public:
    unit_cube_gen(const vertex_attrib_kinds attr_kinds) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void face_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto draw_variant_count() -> span_size_t override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    using _base = generator_base;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    static auto _shared_attrs() noexcept -> vertex_attrib_kinds;

    auto _only_shared_attribs() noexcept -> bool;

    static auto _coord_c(const span_size_t v, const span_size_t c) noexcept
      -> int;

    static auto _normal_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    static auto _tangent_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    static auto _bitangent_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    static auto _face_vert(
      const span_size_t f,
      const span_size_t t,
      const span_size_t v) noexcept -> span_size_t;
};
//------------------------------------------------------------------------------
auto unit_cube(const vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator> {
    return std::make_unique<unit_cube_gen>(attr_kinds);
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::pivot | vertex_attrib_kind::face_coord;
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_shared_attrs() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::pivot |
           vertex_attrib_kind::face_coord;
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_only_shared_attribs() noexcept -> bool {
    return !(attrib_kinds() & ~_shared_attrs());
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_face_vert(
  const span_size_t f,
  const span_size_t t,
  const span_size_t v) noexcept -> span_size_t {
    assert(f < 6);
    assert(t < 2);
    assert(v < 3);

    /*
     *    (2)-----(3)    (Y)
     *    /|      /|      ^
     *   / |     / |      |
     * (6)-|---(7) |      |
     *  | (0)---|-(1)     O----> (X)
     *  | /     | /      /
     *  |/      |/      /
     * (4)-----(5)     L (Z)
     *
     *    (2)----(3)   (2)  (0)---(2)
     *     ^ \ II |     | \   \ II |
     *     |  \   | <=> |  \   \   |
     *     |   \  |     |   \   \  |
     *     | I  \ |     | I  \   \ |
     *    (0)--->(1)   (0)-->(1)  (1)
     *
     *        (-)        (+)
     *    (2)----(6) (7)----(3)
     *     ^ \ II |   ^ \ II |
     *     |  \   |   |  \   |
     * (X) |   \  |   |   \  |
     *     | I  \ |   | I  \ |
     *    (0)--->(4) (5)--->(1)
     *
     *        (-)        (+)
     *    (4)----(5) (2)----(3)
     *     ^ \ II |   ^ \ II |
     *     |  \   |   |  \   |
     * (Y) |   \  |   |   \  |
     *     | I  \ |   | I  \ |
     *    (0)--->(1) (6)--->(7)
     *
     *        (-)        (+)
     *    (3)----(2) (6)----(7)
     *     ^ \ II |   ^ \ II |
     *     |  \   |   |  \   |
     * (Z) |   \  |   |   \  |
     *     | I  \ |   | I  \ |
     *    (1)--->(0) (4)--->(5)
     */

    static const span_size_t ftvi[2][3] = {
      {0, 1, 2}, // ( I)
      {2, 1, 3}  // (II)
    };

    static const span_size_t fv[6][4] = {
      {0, 4, 2, 6}, // (-X)
      {5, 1, 7, 3}, // (+X)
      {0, 1, 4, 5}, // (-Y)
      {6, 7, 2, 3}, // (+Y)
      {1, 0, 3, 2}, // (-Z)
      {4, 5, 6, 7}  // (+Z)
    };
    return fv[f][ftvi[t][v]];
}
//------------------------------------------------------------------------------
unit_cube_gen::unit_cube_gen(const vertex_attrib_kinds attr_kinds) noexcept
  : _base(attr_kinds & _attr_mask(), generator_capability::indexed_drawing) {}
//------------------------------------------------------------------------------
auto unit_cube_gen::vertex_count() -> span_size_t {
    if(_only_shared_attribs()) {
        return 8;
    } else {
        return 6 * 2 * 3;
    }
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_coord_c(const span_size_t v, const span_size_t c) noexcept
  -> int {
    assert(v < 8);
    assert(c < 3);

    static const unsigned char _coord_bits[3] = {0xAA, 0xCC, 0xF0};

    const auto b = static_cast<unsigned char>(1U << unsigned(v));
    return ((_coord_bits[c] & b) == b) ? 1 : 0;
}
//------------------------------------------------------------------------------
void unit_cube_gen::positions(span<float> dest) noexcept {
    span_size_t k = 0;

    assert(dest.size() >= vertex_count() * 3);

    if(_only_shared_attribs()) {
        assert(dest.size() >= 8 * 3);

        for(const auto v : integer_range(8)) {
            for(const auto c : integer_range(3)) {
                dest[k++] = float(_coord_c(v, c)) - 0.5F;
            }
        }
    } else {
        assert(dest.size() >= 6 * 2 * 3 * 3);

        for(const auto f : integer_range(6)) {
            for(const auto t : integer_range(2)) {
                for(const auto i : integer_range(3)) {
                    span_size_t v = _face_vert(f, t, i);
                    for(const auto c : integer_range(3)) {
                        dest[k++] = float(_coord_c(v, c)) - 0.5F;
                    }
                }
            }
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_normal_c(const span_size_t f, const span_size_t c) noexcept
  -> int {
    assert(f < 6);
    assert(c < 3);

    //    f =  5, 4, 3, 2, 1, 0
    //  face: +Z,-Z,+Y,-Y,+X,-X
    //   vec: +Z,-Z,+Y,-Y,+X,-X
    // X c=0:  0  0  0  0  1  1 = 0x03
    // Y c=1:  0  0  1  1  0  0 = 0x0C
    // Z c=2:  1  1  0  0  0  0 = 0x30
    static const unsigned char _vec_bits[3] = {0x03, 0x0C, 0x30};
    // X c=0:  0  0  0  0  1  0 = 0x02
    // Y c=1:  0  0  1  0  0  0 = 0x08
    // Z c=2:  1  0  0  0  0  0 = 0x20
    static const unsigned char _vec_sign[3] = {0x02, 0x08, 0x20};

    const auto b = static_cast<unsigned char>(1U << unsigned(f));
    return (((_vec_bits[c] & b) == b) ? 1 : 0) *
           (((_vec_sign[c] & b) == b) ? 1 : -1);
}
//------------------------------------------------------------------------------
void unit_cube_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    span_size_t n = 2 * 3;

    for(const auto f : integer_range(6)) {
        for([[maybe_unused]] const auto i : integer_range(n)) {
            for(const auto c : integer_range(3)) {
                dest[k++] = float(_normal_c(f, c));
            }
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_tangent_c(const span_size_t f, const span_size_t c) noexcept
  -> int {
    assert(f < 6);
    assert(c < 3);

    //    f =  5, 4, 3, 2, 1, 0
    //  face: +Z,-Z,+Y,-Y,+X,-X
    //   vec: +X,-X,+X,+X,-Z,+Z
    // X c=0:  1  1  1  1  0  0 = 0x3C
    // Y c=1:  0  0  0  0  0  0 = 0x00
    // Z c=2:  0  0  0  0  1  1 = 0x03
    static const unsigned char _vec_bits[3] = {0x3C, 0x00, 0x03};
    // X c=0:  1  0  1  1  0  0 = 0x2C
    // Y c=1:  0  0  0  0  0  0 = 0x00
    // Z c=2:  0  0  0  0  0  1 = 0x01
    static const unsigned char _vec_sign[3] = {0x2C, 0x00, 0x01};

    const auto b = static_cast<unsigned char>(1U << unsigned(f));
    return (((_vec_bits[c] & b) == b) ? 1 : 0) *
           (((_vec_sign[c] & b) == b) ? 1 : -1);
}
//------------------------------------------------------------------------------
void unit_cube_gen::tangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::tangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    span_size_t n = 2 * 3;

    for(const auto f : integer_range(6)) {
        for([[maybe_unused]] const auto i : integer_range(n)) {
            for(const auto c : integer_range(3)) {
                dest[k++] = float(_tangent_c(f, c));
            }
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
auto unit_cube_gen::_bitangent_c(
  const span_size_t f,
  const span_size_t c) noexcept -> int {
    assert(f < 6);
    assert(c < 3);

    //    f =  5, 4, 3, 2, 1, 0
    //  face: +Z,-Z,+Y,-Y,+X,-X
    //   vec: +Y,+Y,-Z,+Z,+Y,+Y
    // X c=0:  0  0  0  0  0  0 = 0x00
    // Y c=1:  1  1  0  0  1  1 = 0x33
    // Z c=2:  0  0  1  1  0  0 = 0x0C
    static const unsigned char _vec_bits[3] = {0x00, 0x33, 0x0C};
    // X c=0:  0  0  0  0  0  0 = 0x00
    // Y c=1:  1  1  0  0  1  1 = 0x33
    // Z c=2:  0  0  0  1  0  0 = 0x04
    static const unsigned char _vec_sign[3] = {0x00, 0x33, 0x04};

    const auto b = static_cast<unsigned char>(1U << unsigned(f));
    return (((_vec_bits[c] & b) == b) ? 1 : 0) *
           (((_vec_sign[c] & b) == b) ? 1 : -1);
}
//------------------------------------------------------------------------------
void unit_cube_gen::bitangents(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::bitangent));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;
    span_size_t n = 2 * 3;

    for(const auto f : integer_range(6)) {
        for([[maybe_unused]] const auto i : integer_range(n)) {
            for(const auto c : integer_range(3)) {
                dest[k++] = float(_bitangent_c(f, c));
            }
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_cube_gen::face_coords(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::face_coord));
    assert(dest.size() >= vertex_count() * 3);

    /*
     *     (0,1,f) (1,1,f)
     *       (2)----(3)
     *        ^ \ II |
     *        |  \   |
     *        |   \  |
     *        | I  \ |
     *       (0)--->(1)
     *     (0,0,f) (1,0,f)
     */

    static const span_size_t ftvi[2][3] = {
      {0, 1, 2}, // ( I)
      {2, 1, 3}  // (II)
    };

    const float uv[4][2] = {{0.F, 0.F}, {1.F, 0.F}, {0.F, 1.F}, {1.F, 1.F}};

    span_size_t k = 0;

    for(const auto f : integer_range(6)) {
        for(const auto& ftv : ftvi) {
            for(const auto v : ftv) {
                dest[k++] = uv[v][0];
                dest[k++] = uv[v][1];
                dest[k++] = float(f);
            }
        }
    }

    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_cube_gen::attrib_values(
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
        case vertex_attrib_kind::face_coord:
            face_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
auto unit_cube_gen::draw_variant_count() -> span_size_t {
    return 2;
}
//------------------------------------------------------------------------------
auto unit_cube_gen::index_type(const drawing_variant var) -> index_data_type {
    if(var == 0) {
        if(_only_shared_attribs()) {
            return index_data_type::unsigned_8;
        }
    } else if(var == 1) {
        return index_data_type::unsigned_8;
    }
    return index_data_type::none;
}
//------------------------------------------------------------------------------
auto unit_cube_gen::index_count(const drawing_variant var) -> span_size_t {
    if(var == 0) {
        if(_only_shared_attribs()) {
            return 6 * 2 * 3;
        }
    } else if(var == 1) {
        return 6 * 8;
    }
    return 0;
}
//------------------------------------------------------------------------------
template <typename T>
void unit_cube_gen::_indices(const drawing_variant var, span<T> dest) noexcept {
    assert(dest.size() >= index_count(var));

    span_size_t k = 0;

    if(var == 0) {
        if(_only_shared_attribs()) {
            for(const auto f : integer_range(6)) {
                for(const auto t : integer_range(2)) {
                    for(const auto v : integer_range(3)) {
                        dest[k++] = T(_face_vert(f, t, v));
                    }
                }
            }
        }
    } else if(var == 1) {
        for(const auto f : integer_range(6)) {
            dest[k++] = T(_face_vert(f, 0, 0));
            dest[k++] = T(_face_vert(f, 0, 2));

            dest[k++] = T(_face_vert(f, 0, 2));
            dest[k++] = T(_face_vert(f, 1, 2));

            dest[k++] = T(_face_vert(f, 1, 2));
            dest[k++] = T(_face_vert(f, 1, 1));

            dest[k++] = T(_face_vert(f, 1, 1));
            dest[k++] = T(_face_vert(f, 0, 0));
        }
    }

    assert(k == index_count(var));
}
//------------------------------------------------------------------------------
void unit_cube_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_cube_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_cube_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto unit_cube_gen::operation_count(const drawing_variant) -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
void unit_cube_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    assert(ops.size() >= operation_count(var));

    if(var == 0) {
        if(_only_shared_attribs()) {
            draw_operation& op = ops[0];
            op.mode = primitive_type::triangles;
            op.idx_type = index_type(var);
            op.first = 0;
            op.count = index_count(var);
            op.primitive_restart = false;
            op.cw_face_winding = false;
        } else {
            draw_operation& op = ops[0];
            op.mode = primitive_type::triangles;
            op.idx_type = index_data_type::none;
            op.first = 0;
            op.count = vertex_count();
            op.primitive_restart = false;
            op.cw_face_winding = false;
        }
    } else if(var == 1) {
        draw_operation& op = ops[0];
        op.mode = primitive_type::lines;
        op.idx_type = index_type(var);
        op.first = 0;
        op.count = index_count(var);
        op.primitive_restart = false;
    }
}
//------------------------------------------------------------------------------
auto unit_cube_gen::bounding_sphere() -> math::sphere<float, true> {
    using std::sqrt;
    return {{0.0F}, float(sqrt(2.F))};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
