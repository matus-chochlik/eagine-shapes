/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_SCREEN_HPP
#define EAGINE_SHAPES_SCREEN_HPP

#include "config/basic.hpp"
#include "gen_base.hpp"
#include <cassert>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered flat 2d screen shape from (-1,-1,0) to (1,1,0)
/// @ingroup shapes
/// @see unit_screen
class unit_screen_gen : public centered_unit_shape_generator_base {
public:
    unit_screen_gen(const vertex_attrib_bits attr_bits) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangentials(span<float> dest) noexcept;

    void bitangentials(span<float> dest) noexcept;

    void face_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    using _base = centered_unit_shape_generator_base;

    static auto _attr_mask() noexcept -> vertex_attrib_bits;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_screen_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
static inline auto unit_screen(const vertex_attrib_bits attr_bits) {
    return std::make_unique<unit_screen_gen>(attr_bits);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/screen.inl>
#endif

#endif // EAGINE_SHAPES_SCREEN_HPP
