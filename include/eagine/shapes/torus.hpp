/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_TORUS_HPP
#define EAGINE_SHAPES_TORUS_HPP

#include "gen_base.hpp"
#include <eagine/config/basic.hpp>
#include <eagine/callable_ref.hpp>
#include <eagine/valid_if/ge0_lt1.hpp>
#include <eagine/valid_if/greater_than.hpp>
#include <array>
#include <cassert>
#include <memory>

namespace eagine::shapes {

/// @brief Generator of centered torus shape with unit diameter.
/// @ingroup shapes
/// @see unit_torus
class unit_torus_gen : public centered_unit_shape_generator_base {
public:
    unit_torus_gen(
      const vertex_attrib_bits attr_bits,
      const valid_if_greater_than<int, 4>& rings,
      const valid_if_greater_than<int, 3>& sections,
      const valid_if_ge0_lt1<float>& radius_ratio) noexcept;

    unit_torus_gen(
      const vertex_attrib_bits attr_bits,
      const valid_if_greater_than<int, 4>& rings,
      const valid_if_greater_than<int, 3>& sections) noexcept
      : unit_torus_gen(attr_bits, rings, sections, 0.5F) {}

    unit_torus_gen(const vertex_attrib_bits attr_bits) noexcept
      : unit_torus_gen(attr_bits, 24, 36) {}

    using offset_getter =
      callable_ref<std::array<double, 3>(span_size_t, span_size_t)>;

    auto vertex_count() -> span_size_t override;

    void vertex_pivots(span<float> dest) noexcept;

    void positions(span<float> dest, const offset_getter) noexcept;

    void normals(span<float> dest, const offset_getter) noexcept;

    void tangentials(span<float> dest, const offset_getter) noexcept;

    void bitangentials(span<float> dest, const offset_getter) noexcept;

    void wrap_coords(span<float> dest) noexcept;

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

    static auto _attr_mask() noexcept -> vertex_attrib_bits;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_torus_gen.
/// @ingroup shapes
static inline auto unit_torus(
  const vertex_attrib_bits attr_bits,
  const valid_if_greater_than<int, 4>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) {
    return std::make_unique<unit_torus_gen>(
      attr_bits, rings, sections, radius_ratio);
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
static inline auto unit_torus(const vertex_attrib_bits attr_bits) {
    return unit_torus(attr_bits, 18, 36, 0.5F);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/torus.inl>
#include <utility>
#endif

#endif // EAGINE_SHAPES_TORUS_HPP
