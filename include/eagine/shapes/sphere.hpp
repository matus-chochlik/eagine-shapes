/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_SPHERE_HPP
#define EAGINE_SHAPES_SPHERE_HPP

#include "gen_base.hpp"
#include <eagine/config/basic.hpp>
#include <eagine/valid_if/greater_than.hpp>
#include <cassert>

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered UV-sphere shape with unit diameter.
/// @ingroup shapes
/// @see unit_sphere
class unit_sphere_gen : public centered_unit_shape_generator_base {
public:
    unit_sphere_gen(
      vertex_attrib_bits attr_bits,
      valid_if_greater_than<int, 2> rings,
      valid_if_greater_than<int, 3> sections) noexcept;

    unit_sphere_gen(vertex_attrib_bits attr_bits) noexcept
      : unit_sphere_gen(attr_bits, 12, 18) {}

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangentials(span<float> dest) noexcept;

    void bitangentials(span<float> dest) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void attrib_values(vertex_attrib_variant, span<float>) override;

    auto index_type(drawing_variant) -> index_data_type override;

    auto index_count(drawing_variant) -> span_size_t override;

    void indices(drawing_variant, span<std::uint8_t> dest) override;

    void indices(drawing_variant, span<std::uint16_t> dest) override;

    void indices(drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(drawing_variant) -> span_size_t override;

    void instructions(drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

    void ray_intersections(
      drawing_variant,
      span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) override;

private:
    using _base = centered_unit_shape_generator_base;

    int _rings;
    int _sections;

    static auto _attr_mask() noexcept -> vertex_attrib_bits;

    template <typename T>
    void _indices(drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
static inline auto unit_sphere(
  vertex_attrib_bits attr_bits,
  valid_if_greater_than<int, 2> rings,
  valid_if_greater_than<int, 3> sections) {
    return std::make_unique<unit_sphere_gen>(
      attr_bits, std::move(rings), std::move(sections));
}
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_icosahedron
/// @see unit_screen
static inline auto unit_sphere(vertex_attrib_bits attr_bits) {
    return unit_sphere(attr_bits, 18, 36);
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/shapes/sphere.inl>
#endif

#endif // EAGINE_SHAPES_SPHERE_HPP
