/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:twisted_torus;

import eagine.core.types;
import eagine.core.valid_if;
import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_screen
export [[nodiscard]] auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds,
  const int twist,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_icosahedron
/// @see unit_screen
export [[nodiscard]] auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds) {
    return unit_twisted_torus(attr_kinds, 12, 38, 12, 0.5F);
}

/// @brief Tries to construct instances of unit_twisted_torus_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_twisted_torus_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

