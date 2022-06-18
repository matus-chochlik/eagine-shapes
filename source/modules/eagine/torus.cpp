/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:torus;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.valid_if;
import eagine.core.utility;
import eagine.core.math;
import :generator;
import <memory>;
import <optional>;

namespace eagine::shapes {

/// @brief Generator of centered torus shape with unit diameter.
/// @ingroup shapes
/// @see unit_torus
export class unit_torus_gen : public centered_unit_shape_generator_base {
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

    void vertex_pivots(span<float> dest) noexcept;

    void positions(span<float> dest, const offset_getter) noexcept;

    void normals(span<float> dest, const offset_getter) noexcept;

    void tangents(span<float> dest, const offset_getter) noexcept;

    void bitangents(span<float> dest, const offset_getter) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void occlusions(span<float> dest, const offset_getter) noexcept;

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
    using _base = centered_unit_shape_generator_base;

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
/// @brief Constructs instances of unit_torus_gen.
/// @ingroup shapes
export auto unit_torus(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 4>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) {
    return std::make_unique<unit_torus_gen>(
      attr_kinds, rings, sections, radius_ratio);
}
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_twisted_torus
/// @see unit_icosahedron
/// @see unit_screen
export auto unit_torus(const vertex_attrib_kinds attr_kinds) {
    return unit_torus(attr_kinds, 18, 36, 0.5F);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

