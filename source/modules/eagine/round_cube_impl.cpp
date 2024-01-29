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
import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.valid_if;
import eagine.core.runtime;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
// unit_round_cube_parameter_cache
//------------------------------------------------------------------------------
class unit_round_cube_parameter_cache {
public:
    using vec3 = math::tvec<float, 3, true>;
    unit_round_cube_parameter_cache(int d);

    auto interpolate(int x1, int y1, int x2, int y2) noexcept -> vec3;
    void fill_square(int x1, int y1, int x2, int y2) noexcept;

    constexpr auto index_of(int x, int y) const noexcept {
        return (_d + 1U) * std_size_t(y) + std_size(x);
    }

    void set(int x, int y, vec3 v) noexcept {
        _values[index_of(x, y)] = v;
    }

    auto get(int x, int y) const noexcept -> vec3 {
        return _values[index_of(x, y)];
    }

    auto has(int x, int y) const noexcept -> bool {
        return get(x, y).z() > 0.F;
    }

private:
    std::size_t _d;
    std::vector<vec3> _values;
};
//------------------------------------------------------------------------------
unit_round_cube_parameter_cache::unit_round_cube_parameter_cache(int d)
  : _d{std_size(d)} {
    _values.resize((_d + 1U) * (_d + 1U), vec3{0.F});
    const float isr3{1.F / std::sqrt(3.F)};
    set(0, 0, {-isr3, -isr3, isr3});
    set(_d, 0, {+isr3, -isr3, isr3});
    set(0, _d, {-isr3, +isr3, isr3});
    set(_d, _d, {+isr3, +isr3, isr3});
    fill_square(0, 0, _d, _d);
}
//------------------------------------------------------------------------------
auto unit_round_cube_parameter_cache::interpolate(
  int x1,
  int y1,
  int x2,
  int y2) noexcept -> vec3 {
    assert(has(x1, y1));
    assert(has(x2, y2));
    return math::normalized(get(x1, y1) + get(x2, y2));
}
//------------------------------------------------------------------------------
void unit_round_cube_parameter_cache::fill_square(
  int x1,
  int y1,
  int x2,
  int y2) noexcept {
    const int xm = std::midpoint(x1, x2);
    const int ym = std::midpoint(y1, y2);
    if(not has(xm, y1)) {
        set(xm, y1, interpolate(x1, y1, x2, y1));
    }
    if(not has(x1, ym)) {
        set(x1, ym, interpolate(x1, y1, x1, y2));
    }
    if(not has(x2, ym)) {
        set(x2, ym, interpolate(x2, y1, x2, y2));
    }
    if(not has(xm, y2)) {
        set(xm, y2, interpolate(x1, y2, x2, y2));
    }
    if(not has(xm, ym)) {
        set(xm, ym, interpolate(x1, ym, x2, ym));
        fill_square(x1, y1, xm, ym);
        fill_square(xm, y1, x2, ym);
        fill_square(x1, ym, xm, y2);
        fill_square(xm, ym, x2, y2);
    }
}
//------------------------------------------------------------------------------
// unit_round_cube_gen
//------------------------------------------------------------------------------
class unit_round_cube_gen : public generator_base {
public:
    unit_round_cube_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_positive<int> divisions) noexcept;

    auto vertex_count() -> span_size_t override;

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override;

    auto parameters(int x, int y) noexcept -> math::tvec<float, 3, true>;
    auto normal(int face, int x, int y) noexcept -> math::tvec<float, 3, true>;

    void _directions(span<float> dest, float len) noexcept;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void face_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

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

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    int _divisions;
    unit_round_cube_parameter_cache _parameters;
};
//------------------------------------------------------------------------------
unit_round_cube_gen::unit_round_cube_gen(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int> divisions) noexcept
  : _base(
      attr_kinds & _attr_mask(),
      generator_capability::indexed_drawing |
        generator_capability::primitive_restart)
  , _divisions{divisions.value() + divisions.value() % 2}
  , _parameters{_divisions} {
    assert(_divisions > 0);
}
//------------------------------------------------------------------------------
auto unit_round_cube(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int> divisions) -> shared_holder<generator> {
    return {hold<unit_round_cube_gen>, attr_kinds, divisions};
}
//------------------------------------------------------------------------------
auto unit_round_cube_from(
  const vertex_attrib_kinds attr_kinds,
  const url& locator,
  main_ctx&) -> shared_holder<generator> {
    if(locator.has_path("/unit_round_cube")) {
        const auto divisions{locator.query().arg_value_as(
          "divisions", std::type_identity<valid_if_positive<int>>{})};
        return unit_round_cube(attr_kinds, divisions.value_or(8));
    }
    return {};
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::pivot | vertex_attrib_kind::face_coord |
           vertex_attrib_kind::wrap_coord | vertex_attrib_kind::box_coord;
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::vertex_count() -> span_size_t {
    return 6 * (_divisions + 1) * (_divisions + 1);
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    if(vav == vertex_attrib_kind::wrap_coord) {
        return 3;
    }
    return generator_base::values_per_vertex(vav);
}
//------------------------------------------------------------------------------
auto unit_round_cube_face_normal(const span_size_t f) noexcept {
    return std::array<math::tvec<float, 3, true>, 6>{{
      {-1.F, 0.F, 0.F},
      {+1.F, 0.F, 0.F},
      {0.F, -1.F, 0.F},
      {0.F, +1.F, 0.F},
      {0.F, 0.F, -1.F},
      {0.F, 0.F, +1.F},
    }}[f];
}
//------------------------------------------------------------------------------
auto unit_round_cube_face_tangent(const span_size_t f) noexcept {
    return std::array<math::tvec<float, 3, true>, 6>{{
      {0.F, 0.F, +1.F},
      {0.F, 0.F, -1.F},
      {+1.F, 0.F, 0.F},
      {+1.F, 0.F, 0.F},
      {-1.F, 0.F, 0.F},
      {+1.F, 0.F, 0.F},
    }}[f];
}
//------------------------------------------------------------------------------
auto unit_round_cube_face_bitangent(const span_size_t f) noexcept {
    return std::array<math::tvec<float, 3, true>, 6>{{
      {0.F, +1.F, 0.F},
      {0.F, +1.F, 0.F},
      {0.F, 0.F, +1.F},
      {0.F, 0.F, -1.F},
      {0.F, +1.F, 0.F},
      {0.F, +1.F, 0.F},
    }}[f];
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::parameters(int x, int y) noexcept
  -> math::tvec<float, 3, true> {
    return _parameters.get(x, y);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::_directions(span<float> dest, float len) noexcept {
    span_size_t l = 0;

    assert(dest.size() >= vertex_count() * 3);

    for(const auto f : integer_range(6)) {
        const auto vx{unit_round_cube_face_tangent(f)};
        const auto vy{unit_round_cube_face_bitangent(f)};
        const auto vz{unit_round_cube_face_normal(f)};
        for(const auto y : integer_range(_divisions + 1)) {
            for(const auto x : integer_range(_divisions + 1)) {
                const auto p{parameters(x, y) * len};
                const auto vp = vx * p.x() + vy * p.y() + vz * p.z();
                dest[l++] = vp.x();
                dest[l++] = vp.y();
                dest[l++] = vp.z();
            }
        }
    }

    assert(l == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::positions(span<float> dest) noexcept {
    _directions(dest, 0.5F);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    _directions(dest, 1.0F);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::tangents(span<float> dest) noexcept {
    span_size_t l = 0;

    assert(dest.size() >= vertex_count() * 3);

    for(const auto f : integer_range(6)) {
        const auto vx{-unit_round_cube_face_normal(f)};
        const auto vy{unit_round_cube_face_bitangent(f)};
        const auto vz{unit_round_cube_face_tangent(f)};
        for(const auto y : integer_range(_divisions + 1)) {
            for(const auto x : integer_range(_divisions + 1)) {
                const auto p{parameters(x, y)};
                const auto vp = vx * p.x() + vy * p.y() + vz * p.z();
                dest[l++] = vp.x();
                dest[l++] = vp.y();
                dest[l++] = vp.z();
            }
        }
    }

    assert(l == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::bitangents(span<float> dest) noexcept {
    span_size_t l = 0;

    assert(dest.size() >= vertex_count() * 3);

    for(const auto f : integer_range(6)) {
        const auto vx{unit_round_cube_face_tangent(f)};
        const auto vy{-unit_round_cube_face_normal(f)};
        const auto vz{unit_round_cube_face_bitangent(f)};
        for(const auto y : integer_range(_divisions + 1)) {
            for(const auto x : integer_range(_divisions + 1)) {
                const auto p{parameters(x, y)};
                const auto vp = vx * p.x() + vy * p.y() + vz * p.z();
                dest[l++] = vp.x();
                dest[l++] = vp.y();
                dest[l++] = vp.z();
            }
        }
    }

    assert(l == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::face_coords(span<float> dest) noexcept {
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    for(const auto f : integer_range(6)) {
        const auto l = _divisions + 1;
        for(const auto y : integer_range(l)) {
            const float j = float(y) / float(_divisions);
            for(const auto x : integer_range(l)) {
                const float i = float(x) / float(_divisions);
                dest[k++] = i;
                dest[k++] = j;
                dest[k++] = float(f);
            }
        }
    }
    assert(k == vertex_count() * 3);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::attrib_values(
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
        case vertex_attrib_kind::wrap_coord:
            face_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::index_count(const drawing_variant) -> span_size_t {
    if(primitive_restart()) {
        return 6 * _divisions * ((_divisions + 1) * 2 + 1);
    }
    return 6 * _divisions * (_divisions + 1) * 2;
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::index_type(const drawing_variant var)
  -> index_data_type {
    if(index_count(var) < span_size(std::numeric_limits<std::uint8_t>::max())) {
        return index_data_type::unsigned_8;
    }
    if(index_count(var) < span_size(std::numeric_limits<std::uint16_t>::max())) {
        return index_data_type::unsigned_16;
    }
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
template <typename T>
void unit_round_cube_gen::_indices(
  [[maybe_unused]] const drawing_variant var,
  span<T> dest) noexcept {
    assert(var == 0);
    assert(dest.size() >= index_count(var));

    span_size_t k = 0;
    for(const auto f : integer_range(6)) {
        for(const auto d : integer_range(_divisions)) {
            for(const auto l : integer_range(_divisions + 1)) {
                for(const auto s : integer_range(2)) {
                    dest[k++] = limit_cast<T>(
                      (f * (_divisions + 1) * (_divisions + 1)) +
                      ((d + s) * (_divisions + 1)) + l);
                }
            }
            if(primitive_restart()) {
                dest[k++] = limit_cast<T>(vertex_count());
            }
        }
    }

    assert(k == index_count(var));
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::operation_count(const drawing_variant)
  -> span_size_t {
    if(primitive_restart()) {
        return 1;
    }
    return 6 * _divisions;
}
//------------------------------------------------------------------------------
void unit_round_cube_gen::instructions(
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
        op.cw_face_winding = true;
        op.primitive_restart = true;
    } else {
        span_size_t o = 0;
        span_size_t offs = 0;
        const span_size_t len = (_divisions + 1) * 2;
        for([[maybe_unused]] const auto f : integer_range(6)) {
            for([[maybe_unused]] const auto d : integer_range(_divisions)) {
                draw_operation& op = ops[o++];
                op.mode = primitive_type::triangle_strip;
                op.idx_type = index_type(var);
                op.first = offs;
                op.count = len;
                op.cw_face_winding = true;
                op.primitive_restart = false;
                offs += len;
            }
        }
        assert(offs == index_count(var));
        assert(o == operation_count(var));
    }
}
//------------------------------------------------------------------------------
auto unit_round_cube_gen::bounding_sphere() -> math::sphere<float, true> {
    using std::sqrt;
    return {{0.0F}, 1.F};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

