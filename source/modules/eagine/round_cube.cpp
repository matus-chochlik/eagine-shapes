/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:round_cube;

import eagine.core.valid_if;
import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_round_cube_gen.
/// @ingroup shapes
/// @see unit_round_cube_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto unit_round_cube(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int> divisions) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
export [[nodiscard]] auto unit_round_cube(const vertex_attrib_kinds attr_kinds) {
    return unit_round_cube(attr_kinds, 8);
}

/// @brief Tries to construct instances of unit_round_cube_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_round_cube_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

