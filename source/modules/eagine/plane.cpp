/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:plane;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.valid_if;
import eagine.core.math;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered flat 2d plane shape.
/// @ingroup shapes
/// @see unit_plane
export class unit_plane_gen : public centered_unit_shape_generator_base {
public:
    unit_plane_gen(
      const vertex_attrib_kinds attr_kinds,
      const valid_if_positive<int>& width,
      const valid_if_positive<int>& height) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void box_coords(span<float> dest) noexcept;

    void vertex_coords(span<std::int32_t> dest) noexcept;

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override;

    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;

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
    using _base = centered_unit_shape_generator_base;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    int _width;
    int _height;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_plane_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_screen
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
export auto unit_plane(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) {
    return std::make_unique<unit_plane_gen>(attr_kinds, width, height);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

