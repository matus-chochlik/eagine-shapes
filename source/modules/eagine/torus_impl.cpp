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
class unit_torus_gen : public generator_base {
public:
    unit_torus_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_greater_than<int, 4>& rings,
      const valid_if_greater_than<int, 3>& sections,
      const valid_if_ge0_lt1<float>& radius_ratio) noexcept;

    unit_torus_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_greater_than<int, 4>& rings,
      const valid_if_greater_than<int, 3>& sections) noexcept
      : unit_torus_gen(attr_kinds, rings, sections, 0.5F) {}

    unit_torus_gen(const vertex_attrib_kinds attr_kinds) noexcept
      : unit_torus_gen(attr_kinds, 24, 36) {}

    using offset_getter =
      callable_ref<std::array<double, 3>(span_size_t, span_size_t)>;

    auto vertex_count() -> span_size_t override;

    void pivot_pivots(span<float> dest) noexcept;

    void vertex_pivots(span<float> dest) noexcept;

    void positions(span<float> dest, const offset_getter) noexcept;

    void normals(span<float> dest, const offset_getter) noexcept;

    void tangents(span<float> dest, const offset_getter) noexcept;

    void bitangents(span<float> dest, const offset_getter) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void roughness(span<float> dest, const offset_getter) noexcept;
    void occlusions(span<float> dest, const offset_getter) noexcept;
    void opposite_lengths(span<float> dest, const offset_getter) noexcept;
    void edge_lengths(span<float> dest, const offset_getter) noexcept;
    void face_areas(span<float> dest, const offset_getter) noexcept;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override;

    auto variant_name(const vertex_attrib_variant) -> string_view override;

    auto special_variant_name(const span_size_t index) -> string_view;
    void make_special_attrib_values(
      void (unit_torus_gen::*const function)(
        span<float>,
        unit_torus_gen::offset_getter),
      const span_size_t,
      span<float>);

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

    const std::size_t _r_seed{1234};
    const std::size_t _s_seed{2345};
    int _rings;
    int _sections;
    float _radius_ratio;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
auto unit_torus_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::tangent | vertex_attrib_kind::bitangent |
           vertex_attrib_kind::roughness | vertex_attrib_kind::occlusion |
           vertex_attrib_kind::opposite_length |
           vertex_attrib_kind::edge_length | vertex_attrib_kind::face_area |
           vertex_attrib_kind::pivot | vertex_attrib_kind::pivot_pivot |
           vertex_attrib_kind::vertex_pivot | vertex_attrib_kind::box_coord |
           vertex_attrib_kind::wrap_coord;
}
//------------------------------------------------------------------------------
auto unit_torus_from(
  const vertex_attrib_kinds attr_kinds,
  const url& locator,
  main_ctx&) -> shared_holder<generator> {
    if(locator.has_path("/unit_torus")) {
        const auto rings{locator.query().arg_value_as(
          "rings", std::type_identity<valid_if_greater_than<int, 4>>{})};
        const auto sections{locator.query().arg_value_as(
          "sections", std::type_identity<valid_if_greater_than<int, 3>>{})};
        const auto radius_ratio{locator.query().arg_value_as(
          "radius_ratio", std::type_identity<valid_if_ge0_lt1<float>>{})};
        return unit_torus(
          attr_kinds,
          rings.value_or(18),
          sections.value_or(36),
          radius_ratio.value_or(0.5F));
    }
    return {};
}
//------------------------------------------------------------------------------
auto unit_torus(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 4>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) -> shared_holder<generator> {
    return {hold<unit_torus_gen>, attr_kinds, rings, sections, radius_ratio};
}
//------------------------------------------------------------------------------
unit_torus_gen::unit_torus_gen(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 4>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) noexcept
  : _base(
      attr_kinds & _attr_mask(),
      generator_capability::indexed_drawing |
        generator_capability::primitive_restart |
        generator_capability::element_strips)
  , _rings{rings.value()}
  , _sections{sections.value()}
  , _radius_ratio(radius_ratio.value()) {}
//------------------------------------------------------------------------------
auto unit_torus_gen::vertex_count() -> span_size_t {
    return (_rings + 1) * (_sections + 1);
}
//------------------------------------------------------------------------------
void unit_torus_gen::pivot_pivots(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::position));
    assert(dest.size() >= vertex_count() * 3);

    fill(dest, 0.F);
}
//------------------------------------------------------------------------------
void unit_torus_gen::vertex_pivots(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::position));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const auto ro = 0.25;
    const auto ri = ro * _radius_ratio;
    const auto rc = (ro + ri) / 2;

    const auto s_step = math::tau / _sections;

    for(const auto s : integer_range(_sections + 1)) {
        const auto vx = std::cos(s * s_step) * rc;
        const auto vz = -std::sin(s * s_step) * rc;

        for([[maybe_unused]] const auto r : integer_range(_rings + 1)) {
            dest[k++] = float(vx);
            dest[k++] = float(0);
            dest[k++] = float(vz);
        }
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::positions(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::position));
    assert(dest.size() >= vertex_count() * 3);

    const auto ro = 0.25;
    const auto ri = ro * _radius_ratio;
    const auto r1 = ri;
    const auto r2 = ro - ri;

    const auto s_step = math::tau / _sections;
    const auto r_step = math::tau / _rings;

    const auto k{[this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    }};

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);

            const auto vr = -std::cos((r + rd) * r_step);
            const auto vx = std::cos((s + sd) * s_step);
            const auto vy = std::sin((r + rd) * r_step);
            const auto vz = -std::sin((s + sd) * s_step);
            const auto rt = r2 * (1 + td);

            dest[k(s, r, 0)] = float(vx * (r1 + rt * (1 + vr)));
            dest[k(s, r, 1)] = float(vy * rt);
            dest[k(s, r, 2)] = float(vz * (r1 + rt * (1 + vr)));
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::normals(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    const auto s_step = math::tau / _sections;
    const auto r_step = math::tau / _rings;

    const auto k{[this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    }};

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);

            const auto nr = -std::cos((r + rd) * r_step);
            const auto nx = std::cos((s + sd) * s_step);
            const auto ny = std::sin((r + rd) * r_step);
            const auto nz = -std::sin((s + sd) * s_step);

            dest[k(s, r, 0)] = float(nx * nr);
            dest[k(s, r, 1)] = float(ny);
            dest[k(s, r, 2)] = float(nz * nr);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::tangents(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::tangent));
    assert(dest.size() >= vertex_count() * 3);

    const auto k{[this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    }};

    const auto s_step = math::tau / _sections;

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(rd);
            (void)(td);

            const auto tx = -std::sin((s + sd) * s_step);
            const auto tz = -std::cos((s + sd) * s_step);

            dest[k(s, r, 0)] = float(tx);
            dest[k(s, r, 1)] = float(0);
            dest[k(s, r, 2)] = float(tz);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::bitangents(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::bitangent));
    assert(dest.size() >= vertex_count() * 3);

    const auto k{[this](span_size_t s, span_size_t r, span_size_t c) {
        return 3 * (s * (_rings + 1) + r) + c;
    }};

    const auto s_step = math::tau / _sections;
    const auto r_step = math::tau / _rings;

    const auto ty = 0;

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);

            const auto tx = -std::sin((s + sd) * s_step);
            const auto tz = -std::cos((s + sd) * s_step);
            const auto nr = -std::cos((r + rd) * r_step);
            const auto ny = std::sin((r + rd) * r_step);
            const auto nx = -tz * nr;
            const auto nz = tx * nr;

            dest[k(s, r, 0)] = float(ny * tz - nz * ty);
            dest[k(s, r, 1)] = float(nz * tx - nx * tz);
            dest[k(s, r, 2)] = float(nx * ty - ny * tx);
        }
        dest[k(s, _rings, 0)] = dest[k(s, 0, 0)];
        dest[k(s, _rings, 1)] = dest[k(s, 0, 1)];
        dest[k(s, _rings, 2)] = dest[k(s, 0, 2)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r, 0)] = dest[k(0, r, 0)];
        dest[k(_sections, r, 1)] = dest[k(0, r, 1)];
        dest[k(_sections, r, 2)] = dest[k(0, r, 2)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::wrap_coords(span<float> dest) noexcept {
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
void unit_torus_gen::roughness(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::roughness));
    assert(dest.size() >= vertex_count());

    const auto r_step = math::pi / _rings;

    const auto k{[this](span_size_t s, span_size_t r) {
        return (s * (_rings + 1) + r);
    }};

    const auto blend_fact = float(std::exp(-_radius_ratio * 0.3F));

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);
            (void)(sd);

            const auto s_in{
              math::minimum(float(std::sin((r + rd) * r_step)) * 1.5F, 1.F)};
            const auto s_out{float(std::sin((r + rd) * r_step)) * 0.9F};
            dest[k(s, r)] =
              math::maximum(1.F - math::blend(1.F, s_in, blend_fact), s_out);
        }
        dest[k(s, _rings)] = dest[k(s, 0)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r)] = dest[k(0, r)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::occlusions(
  span<float> dest,
  const unit_torus_gen::offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::occlusion));
    assert(dest.size() >= vertex_count());

    const auto r_step = math::pi / _rings;

    const auto k{[this](span_size_t s, span_size_t r) {
        return (s * (_rings + 1) + r);
    }};

    const auto blend_fact = float(std::exp(-_radius_ratio * 0.3F));

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);
            (void)(sd);

            dest[k(s, r)] = math::blend(
              1.F,
              math::minimum(float(std::sin((r + rd) * r_step)) * 1.5F, 1.F),
              blend_fact);
        }
        dest[k(s, _rings)] = dest[k(s, 0)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r)] = dest[k(0, r)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::opposite_lengths(
  span<float> dest,
  const offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::opposite_length));
    assert(dest.size() >= vertex_count());

    const auto r_step = math::pi / _rings;
    const auto s_step = math::pi / _sections;

    const auto k{[this](span_size_t s, span_size_t r) {
        return (s * (_rings + 1) + r);
    }};

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);
            (void)(sd);
            const float l{float(s_step * std::sin((r + rd) * r_step))};

            dest[k(s, r)] = l;
        }
        dest[k(s, _rings)] = dest[k(s, 0)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r)] = dest[k(0, r)];
    }
}
//------------------------------------------------------------------------------
void unit_torus_gen::edge_lengths(
  span<float> dest,
  const offset_getter) noexcept {
    assert(has(vertex_attrib_kind::edge_length));
    assert(dest.size() >= 3 * vertex_count());
}
//------------------------------------------------------------------------------
void unit_torus_gen::face_areas(
  span<float> dest,
  const offset_getter get_offs) noexcept {
    assert(has(vertex_attrib_kind::face_area));
    assert(dest.size() >= vertex_count());

    const auto r_step = math::pi / _rings;
    const auto s_step = math::pi / _sections;
    const auto a_mult =
      std::sqrt(std::pow(r_step, 2.F) + std::pow(s_step, 2.F));

    const auto k{[this](span_size_t s, span_size_t r) {
        return (s * (_rings + 1) + r);
    }};

    for(const auto s : integer_range(_sections)) {
        for(const auto r : integer_range(_rings)) {
            const auto [rd, sd, td] = get_offs(s, r);
            (void)(td);
            (void)(sd);

            dest[k(s, r)] =
              float(a_mult * std::pow(std::sin((r + rd) * r_step), 2.F));
        }
        dest[k(s, _rings)] = dest[k(s, 0)];
    }
    for(const auto r : integer_range(_rings + 1)) {
        dest[k(_sections, r)] = dest[k(0, r)];
    }
}
//------------------------------------------------------------------------------
auto unit_torus_gen::special_variant_name(span_size_t index) -> string_view {
    if(index == 1) {
        return {"random"};
    }
    return {};
}
//------------------------------------------------------------------------------
void unit_torus_gen::make_special_attrib_values(
  void (unit_torus_gen::*const function)(
    span<float>,
    unit_torus_gen::offset_getter),
  const span_size_t variant_index,
  span<float> dest) {
    if(variant_index == 1) {
        auto get_offs{
          [rrg{std::mt19937{_r_seed}},
           srg{std::mt19937{_s_seed}},
           rnd{std::normal_distribution<float>{0.F, 0.15F}},
           snd{std::normal_distribution<float>{0.F, 0.15F}}](
            span_size_t, span_size_t) mutable -> std::array<double, 3> {
              return {{rnd(rrg), snd(srg), 0.0}};
          }};
        (this->*function)(dest, {construct_from, get_offs});
    } else if(variant_index == 2) {
        const auto get_offs{
          [](span_size_t s, span_size_t) -> std::array<double, 3> {
              return {{double(s), 0.0, 0.0}};
          }};
        (this->*function)(dest, {construct_from, get_offs});
    } else if(variant_index == 3) {
        auto get_offs =
          [this](span_size_t s, span_size_t r) -> std::array<double, 3> {
            const auto x = float(s) / float(this->_sections);
            const auto y = float(r) / float(this->_rings);
            return {{math::sine_wave01(x), math::sine_wave01(y), 0.0}};
        };
        (this->*function)(dest, {construct_from, get_offs});
    } else {
        const auto no_offs =
          [](span_size_t, span_size_t) -> std::array<double, 3> {
            return {{0.0, 0.0, 0.0}};
        };
        (this->*function)(dest, offset_getter{no_offs});
    }
}
//------------------------------------------------------------------------------
auto unit_torus_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    switch(attrib) {
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::vertex_pivot:
            break;
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangent:
        case vertex_attrib_kind::bitangent:
            return 4;
        case vertex_attrib_kind::roughness:
        case vertex_attrib_kind::occlusion:
            return 1;
        default:
            break;
    }
    return _base::attribute_variants(attrib);
}
//------------------------------------------------------------------------------
auto unit_torus_gen::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    switch(vav.attribute()) {
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangent:
        case vertex_attrib_kind::bitangent:
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::roughness:
        case vertex_attrib_kind::occlusion:
            return special_variant_name(vav.index());
        default:
            break;
    }
    return generator_base::variant_name(vav);
}
//------------------------------------------------------------------------------
void unit_torus_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::pivot_pivot:
            pivot_pivots(dest);
            break;
        case vertex_attrib_kind::vertex_pivot:
            vertex_pivots(dest);
            break;
        case vertex_attrib_kind::position:
            make_special_attrib_values(
              &unit_torus_gen::positions, vav.index(), dest);
            break;
        case vertex_attrib_kind::normal:
            make_special_attrib_values(
              &unit_torus_gen::normals, vav.index(), dest);
            break;
        case vertex_attrib_kind::tangent:
            make_special_attrib_values(
              &unit_torus_gen::tangents, vav.index(), dest);
            break;
        case vertex_attrib_kind::bitangent:
            make_special_attrib_values(
              &unit_torus_gen::bitangents, vav.index(), dest);
            break;
        case vertex_attrib_kind::roughness:
            make_special_attrib_values(
              &unit_torus_gen::roughness, vav.index(), dest);
            break;
        case vertex_attrib_kind::occlusion:
            make_special_attrib_values(
              &unit_torus_gen::occlusions, vav.index(), dest);
            break;
        case vertex_attrib_kind::opposite_length:
            make_special_attrib_values(
              &unit_torus_gen::opposite_lengths, vav.index(), dest);
            break;
        case vertex_attrib_kind::edge_length:
            make_special_attrib_values(
              &unit_torus_gen::edge_lengths, vav.index(), dest);
            break;
        case vertex_attrib_kind::face_area:
            make_special_attrib_values(
              &unit_torus_gen::face_areas, vav.index(), dest);
            break;
        case vertex_attrib_kind::wrap_coord:
            wrap_coords(dest);
            break;
        default:
            generator_base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
auto unit_torus_gen::draw_variant_count() -> span_size_t {
    return 2;
}
//------------------------------------------------------------------------------
auto unit_torus_gen::index_type(const drawing_variant v) -> index_data_type {
    if(index_count(v) < span_size(std::numeric_limits<std::uint8_t>::max())) {
        return index_data_type::unsigned_8;
    }
    if(index_count(v) < span_size(std::numeric_limits<std::uint16_t>::max())) {
        return index_data_type::unsigned_16;
    }
    return index_data_type::unsigned_32;
}
//------------------------------------------------------------------------------
auto unit_torus_gen::index_count(const drawing_variant var) -> span_size_t {
    if(var == 0) {
        return _sections * ((_rings + 1) * 2 + (primitive_restart() ? 1 : 0));
    }
    if(var == 1) {
        return (_sections * (_rings + (primitive_restart() ? 1 : 0))) +
               (_rings * (_sections + (primitive_restart() ? 1 : 0)));
    }
    return 0;
}
//------------------------------------------------------------------------------
template <typename T>
void unit_torus_gen::_indices(const drawing_variant var, span<T> dest) noexcept {
    assert(dest.size() >= index_count(var));

    const auto pri = limit_cast<T>(vertex_count());
    span_size_t k = 0;

    if(var == 0) {
        span_size_t step = _rings + 1;
        for(const auto s : integer_range(_sections)) {
            for(const auto r : integer_range(step)) {
                dest[k++] = limit_cast<T>((s + 0) * step + r);
                dest[k++] = limit_cast<T>((s + 1) * step + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
    } else if(var == 1) {
        for(const auto s : integer_range(_sections)) {
            for(const auto r : integer_range(_rings)) {
                dest[k++] = limit_cast<T>(s * (_rings + 1) + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
        for(const auto r : integer_range(_rings)) {
            for(const auto s : integer_range(_sections)) {
                dest[k++] = limit_cast<T>(s * (_rings + 1) + r);
            }

            if(primitive_restart()) {
                dest[k++] = pri;
            }
        }
    }
    assert(k == index_count(var));
}
//------------------------------------------------------------------------------
void unit_torus_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_torus_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void unit_torus_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto unit_torus_gen::operation_count(const drawing_variant var) -> span_size_t {
    if(var == 0) {
        if(primitive_restart()) {
            return 1;
        } else {
            return _sections;
        }
    }
    if(var == 1) {
        if(primitive_restart()) {
            return 1;
        } else {
            return _sections + _rings;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
void unit_torus_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    assert(ops.size() >= operation_count(var));

    if(var == 0) {
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
    } else if(var == 1) {
        if(primitive_restart()) {
            draw_operation& op = ops[0];
            op.mode = primitive_type::line_loop;
            op.idx_type = index_type(var);
            op.first = 0;
            op.count = index_count(var);
            op.primitive_restart_index = unsigned(vertex_count());
            op.primitive_restart = true;
        } else {
            span_size_t k = 0;
            for(const auto s : integer_range(_sections)) {
                draw_operation& op = ops[k++];
                op.mode = primitive_type::line_loop;
                op.idx_type = index_type(var);
                op.first = s * _rings;
                op.count = _rings;
                op.primitive_restart = false;
            }
            for(const auto r : integer_range(_rings)) {
                draw_operation& op = ops[k++];
                op.mode = primitive_type::line_loop;
                op.idx_type = index_type(var);
                op.first = r * _sections;
                op.count = _sections;
                op.primitive_restart = false;
            }
        }
    }
}
//------------------------------------------------------------------------------
auto unit_torus_gen::bounding_sphere() -> math::sphere<float, true> {
    return {{0.0F}, 0.5F};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

