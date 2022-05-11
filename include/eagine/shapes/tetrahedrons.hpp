/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_TETRAHEDRONS_HPP
#define EAGINE_SHAPES_TETRAHEDRONS_HPP

#include "config/basic.hpp"
#include "generator.hpp"
#include <cassert>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of single marching tetrahedrons cubic cell.
/// @ingroup shapes
/// @see marching_tetrahedrons
class marching_tetrahedrons_gen : public generator_base {
public:
    marching_tetrahedrons_gen(const vertex_attrib_kinds attr_kinds) noexcept;

    auto vertex_count() -> span_size_t override;

    void coords(span<std::int16_t> dest) noexcept;

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override;

    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) override;

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

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of marching_tetrahedrons_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_torus
/// @see unit_screen
/// @see unit_twisted_torus
static inline auto marching_tetrahedrons(vertex_attrib_kinds attr_kinds) {
    return std::make_unique<marching_tetrahedrons_gen>(attr_kinds);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/tetrahedrons.inl>
#endif

#endif
