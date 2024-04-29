/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:drawing;

import std;
import eagine.core.types;
import eagine.core.identifier;
import eagine.core.reflection;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief The shape primitive type enumeration.
/// @ingroup shapes
export enum class primitive_type : std::uint8_t {
    /// @brief Individual points.
    points,
    /// @brief Individual line segments.
    lines,
    /// @brief Line strips.
    line_strip,
    /// @brief Close line loops.
    line_loop,
    /// @brief Individual triangles.
    triangles,
    /// @brief Triangle strips.
    triangle_strip,
    /// @brief Triangle fans.
    triangle_fan,
    /// @brief Individual triangles with adjacency.
    triangles_adjacency,
    /// @brief Quadrilaterals (represented as lines with adjacency).
    quads,
    /// @brief Tetrahedrons (represented as lines with adjacency).
    tetrahedrons,
    /// @brief Patches.
    patches
};
//------------------------------------------------------------------------------
/// @brief Shape vertex attribute data type enumeration.
/// @ingroup shapes
/// @see index_data_type
export enum class attrib_data_type : std::uint8_t {
    /// @brief None / unknown.
    none,
    /// @brief Unsigned byte.
    ubyte,
    /// @brief Signed 16-bit integer.
    int_16,
    /// @brief Signed 32-bit integer.
    int_32,
    /// @brief Unsigned 16-bit integer.
    uint_16,
    /// @brief Unsigned 32-bit integer.
    uint_32,
    /// @brief Floating-point.
    float_
};
//------------------------------------------------------------------------------
/// @brief Shape element index type enumeration.
/// @ingroup shapes
/// @see attrib_data_type
export enum class index_data_type : std::uint8_t {
    /// @brief None / unknown.
    none = 0,
    /// @brief Unsigned 8-bit integer.
    unsigned_8 = 8,
    /// @brief Unsigned 16-bit integer.
    unsigned_16 = 16,
    /// @brief Unsigned 32-bit integer.
    unsigned_32 = 32
};
//------------------------------------------------------------------------------
} // namespace shapes
export template <>
struct enumerator_traits<shapes::primitive_type> {
    static auto mapping() noexcept
      -> enumerator_map_type<shapes::primitive_type, 11>;
};
//------------------------------------------------------------------------------
export template <>
struct enumerator_traits<shapes::attrib_data_type> {
    static auto mapping() noexcept
      -> enumerator_map_type<shapes::attrib_data_type, 7>;
};
//------------------------------------------------------------------------------
export template <>
struct enumerator_traits<shapes::index_data_type> {
    static auto mapping() noexcept
      -> enumerator_map_type<shapes::index_data_type, 4>;
};
namespace shapes {
//------------------------------------------------------------------------------
export auto operator<(
  const index_data_type l,
  const index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) < UT(r);
}
//------------------------------------------------------------------------------
export auto operator>(
  const index_data_type l,
  const index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) > UT(r);
}
//------------------------------------------------------------------------------
export auto operator<=(
  const index_data_type l,
  const index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) <= UT(r);
}
//------------------------------------------------------------------------------
export auto operator>=(
  const index_data_type l,
  const index_data_type r) noexcept {
    using UT = std::underlying_type_t<index_data_type>;
    return UT(l) >= UT(r);
}
//------------------------------------------------------------------------------
/// @brief Draw operation parameters.
/// @ingroup shapes
export struct draw_operation {
    /// @brief The index of the first element.
    span_size_t first{0};

    /// @brief The count of elements.
    span_size_t count{0};

    /// @brief The drawing phase (shader-dependent).
    std::uint32_t phase{0};

    /// @brief The primitive restart index value.
    std::uint32_t primitive_restart_index{0};

    /// @brief The number of vertices per patch.
    std::uint16_t patch_vertices{3};

    /// @brief The primitive type.
    primitive_type mode{primitive_type::points};

    /// @brief The index data type.
    index_data_type idx_type{index_data_type::none};

    /// @brief Whether primitive restart is enabled.
    bool primitive_restart : 1 {false};

    /// @brief Whether the shape faces have clockwise winding (or counterclockwise).
    bool cw_face_winding : 1 {false};
};
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine
