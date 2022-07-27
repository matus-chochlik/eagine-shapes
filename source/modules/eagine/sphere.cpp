/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:sphere;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.valid_if;
import eagine.core.math;
import :generator;
import <memory>;
import <optional>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export auto unit_sphere(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_icosahedron
/// @see unit_screen
export auto unit_sphere(const vertex_attrib_kinds attr_kinds) {
    return unit_sphere(attr_kinds, 18, 36);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

