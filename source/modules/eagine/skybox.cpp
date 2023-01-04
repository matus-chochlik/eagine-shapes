/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:skybox;

import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of skybox_gen
/// @ingroup shapes
/// @see skybox_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto skybox(const vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator>;

/// @brief Tries to construct instances of skybox_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto skybox_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

