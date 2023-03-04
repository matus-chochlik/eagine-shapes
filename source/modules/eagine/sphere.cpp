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
import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see unit_sphere_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto unit_sphere(
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
export [[nodiscard]] auto unit_sphere(const vertex_attrib_kinds attr_kinds) {
    return unit_sphere(attr_kinds, 18, 36);
}

/// @brief Tries to construct instances of unit_sphere_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_sphere_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

