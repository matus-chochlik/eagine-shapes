/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:generator_capabilities;

import eagine.core.types;
import eagine.core.reflection;
import std;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief Shape generator capability bit enumeration.
/// @ingroup shapes
/// @see generator_capabilities
export enum class generator_capability : std::uint16_t {
    /// @brief Indexed drawing of elements should be used if possible.
    indexed_drawing = 1U << 0U,
    /// @brief Line or triangle strips should be generated if possible.
    element_strips = 1U << 1U,
    /// @brief Line or triangle fanst should be generated if possible.
    element_fans = 1U << 2U,
    /// @brief Primitive restart functionality should be used if possible.
    primitive_restart = 1U << 3U,
    /// @brief Vertex attrib divisor functionality.
    attrib_divisors = 1U << 4U
};
//------------------------------------------------------------------------------
/// @brief Alias for generator_capability bitfield type.
/// @ingroup shapes
export using generator_capabilities = bitfield<generator_capability>;
//------------------------------------------------------------------------------
/// @brief Returns vertex_attrib_kinds value with all bits set.
/// @ingroup shapes
export [[nodiscard]] constexpr auto all_generator_capabilities() noexcept
  -> generator_capabilities {
    return generator_capabilities{(1U << 5U) - 1U};
}
//------------------------------------------------------------------------------
/// @brief Bitwise-or operator for generator_capability enumerators.
/// @ingroup shapes
export [[nodiscard]] constexpr auto operator|(
  const generator_capability a,
  const generator_capability b) noexcept -> generator_capabilities {
    return {a, b};
}
//------------------------------------------------------------------------------
} // namespace shapes
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<shapes::generator_capability>,
  const Selector) noexcept {
    using shapes::generator_capability;
    return enumerator_map_type<generator_capability, 5>{
      {{"indexed_drawing", generator_capability::indexed_drawing},
       {"element_strips", generator_capability::element_strips},
       {"element_fans", generator_capability::element_fans},
       {"primitive_restart", generator_capability::primitive_restart},
       {"attrib_divisors", generator_capability::attrib_divisors}}};
}
//------------------------------------------------------------------------------
} // namespace eagine
