/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_SKYBOX_HPP
#define EAGINE_SHAPES_SKYBOX_HPP

#include "config/basic.hpp"
#include "generator.hpp"
#include <cassert>
#include <memory>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered cube shape with unit edge size.
/// @ingroup shapes
/// @see unit_cube
class skybox_gen : public centered_unit_shape_generator_base {
public:
    skybox_gen(const vertex_attrib_kinds attr_kinds) noexcept;

    auto vertex_count() -> span_size_t override;

    void face_coords(span<float> dest) noexcept;

    void positions(span<float> dest) noexcept;

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

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of skybox_gen
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
static inline auto skybox(const vertex_attrib_kinds attr_kinds) {
    return std::make_unique<skybox_gen>(attr_kinds);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/skybox.inl>
#endif

#endif
