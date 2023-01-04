/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:icosahedron;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_icosahedron_gen.
/// @ingroup shapes
/// @see unit_icosahedron_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_torus
/// @see unit_screen
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_icosahedron(vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator>;

/// @brief Tries to construct instances of unit_icosahedron_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_icosahedron_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

