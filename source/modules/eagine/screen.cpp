/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:screen;

import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_screen_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_screen(const vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator>;

/// @brief Tries to construct instances of unit_screen_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_screen_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

