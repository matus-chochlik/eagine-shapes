/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:round_cube;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered, subdivided and rounded cube shape with unit radius.
/// @ingroup shapes
/// @see unit_round_cube
export class unit_round_cube_gen : public centered_unit_shape_generator_base {
public:
    unit_round_cube_gen(
      const vertex_attrib_kinds attr_kinds,
      const int divisions) noexcept;

    auto vertex_count() -> span_size_t override;

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
    using _base = centered_unit_shape_generator_base;

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    int _divisions;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_round_cube_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export auto unit_round_cube(
  const vertex_attrib_kinds attr_kinds,
  const int divisions) {
    return std::make_unique<unit_round_cube_gen>(attr_kinds, divisions);
}
//------------------------------------------------------------------------------
export auto unit_round_cube(const vertex_attrib_kinds attr_kinds) {
    return std::make_unique<unit_round_cube_gen>(attr_kinds, 8);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

