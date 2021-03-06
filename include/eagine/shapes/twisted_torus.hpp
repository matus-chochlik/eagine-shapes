/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_TWISTED_TORUS_HPP
#define EAGINE_SHAPES_TWISTED_TORUS_HPP

#include "config/basic.hpp"
#include "generator.hpp"
#include <eagine/valid_if/ge0_lt1.hpp>
#include <eagine/valid_if/greater_than.hpp>
#include <cassert>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered UV-sphere shape with unit diameter.
/// @ingroup shapes
/// @see unit_twisted_torus
class unit_twisted_torus_gen : public centered_unit_shape_generator_base {
public:
    unit_twisted_torus_gen(
      const vertex_attrib_kinds attr_kinds,
      const int twist,
      const valid_if_greater_than<int, 2>& rings,
      const valid_if_greater_than<int, 3>& sections,
      const valid_if_ge0_lt1<float>& radius_ratio) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    using _base = centered_unit_shape_generator_base;

    int _twist;
    int _rings;
    int _sections;
    float _radius_ratio{0.50F};
    float _thickness_ratio{0.1F};

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_screen
static inline auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds,
  const int twist,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) {
    return std::make_unique<unit_twisted_torus_gen>(
      attr_kinds, twist, rings, sections, radius_ratio);
}
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_icosahedron
/// @see unit_screen
static inline auto unit_twisted_torus(const vertex_attrib_kinds attr_kinds) {
    return unit_twisted_torus(attr_kinds, 12, 38, 12, 0.5F);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/twisted_torus.inl>
#endif

#endif // EAGINE_SHAPES_TWISTED_TORUS_HPP
