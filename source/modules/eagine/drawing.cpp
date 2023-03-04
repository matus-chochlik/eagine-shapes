/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:drawing;

import eagine.core.types;
import eagine.core.identifier;
import eagine.core.reflection;
import std;

namespace eagine::shapes {
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
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<primitive_type>,
  const Selector) noexcept {
    return enumerator_map_type<primitive_type, 11>{
      {{"points", primitive_type::points},
       {"lines", primitive_type::lines},
       {"line_strip", primitive_type::line_strip},
       {"line_loop", primitive_type::line_loop},
       {"triangles", primitive_type::triangles},
       {"triangle_strip", primitive_type::triangle_strip},
       {"triangle_fan", primitive_type::triangle_fan},
       {"triangles_adjacency", primitive_type::triangles_adjacency},
       {"quads", primitive_type::quads},
       {"tetrahedrons", primitive_type::tetrahedrons},
       {"patches", primitive_type::patches}}};
}
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
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<attrib_data_type>,
  const Selector) noexcept {
    return enumerator_map_type<attrib_data_type, 7>{
      {{"none", attrib_data_type::none},
       {"ubyte", attrib_data_type::ubyte},
       {"int_16", attrib_data_type::int_16},
       {"int_32", attrib_data_type::int_32},
       {"uint_16", attrib_data_type::uint_16},
       {"uint_32", attrib_data_type::uint_32},
       {"float_", attrib_data_type::float_}}};
}
//------------------------------------------------------------------------------
export constexpr auto enumerator_mapping(
  const std::type_identity<attrib_data_type>,
  const from_value_tree_t) noexcept {
    return enumerator_map_type<attrib_data_type, 6>{
      {{"ubyte", attrib_data_type::ubyte},
       {"int_16", attrib_data_type::int_16},
       {"int_32", attrib_data_type::int_32},
       {"uint_16", attrib_data_type::uint_16},
       {"uint_32", attrib_data_type::uint_32},
       {"float", attrib_data_type::float_}}};
}
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
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<index_data_type>,
  const Selector) noexcept {
    return enumerator_map_type<index_data_type, 4>{
      {{"none", index_data_type::none},
       {"unsigned_8", index_data_type::unsigned_8},
       {"unsigned_16", index_data_type::unsigned_16},
       {"unsigned_32", index_data_type::unsigned_32}}};
}
//------------------------------------------------------------------------------
export constexpr auto enumerator_mapping(
  const std::type_identity<index_data_type>,
  const from_value_tree_t) noexcept {
    return enumerator_map_type<index_data_type, 3>{
      {{"none", index_data_type::none},
       {"unsigned_16", index_data_type::unsigned_16},
       {"unsigned_32", index_data_type::unsigned_32}}};
}
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
} // namespace eagine::shapes

