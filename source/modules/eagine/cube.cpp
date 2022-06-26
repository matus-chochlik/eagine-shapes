/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:cube;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator of centered cube shape with unit edge size.
/// @ingroup shapes
/// @see unit_cube
export class unit_cube_gen : public centered_unit_shape_generator_base {
public:
    unit_cube_gen(const vertex_attrib_kinds attr_kinds) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void tangents(span<float> dest) noexcept;

    void bitangents(span<float> dest) noexcept;

    void face_coords(span<float> dest) noexcept;

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

    static auto _shared_attrs() noexcept -> vertex_attrib_kinds;

    auto _only_shared_attribs() noexcept -> bool;

    static auto _coord_c(const span_size_t v, const span_size_t c) noexcept
      -> int;

    static auto _normal_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    static auto _tangent_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    static auto _bitangent_c(const span_size_t f, const span_size_t c) noexcept
      -> int;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    static auto _face_vert(
      const span_size_t f,
      const span_size_t t,
      const span_size_t v) noexcept -> span_size_t;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_cube_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export auto unit_cube(const vertex_attrib_kinds attr_kinds) {
    return std::make_unique<unit_cube_gen>(attr_kinds);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

