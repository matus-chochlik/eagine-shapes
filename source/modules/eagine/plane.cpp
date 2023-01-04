/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:plane;

import eagine.core.types;
import eagine.core.valid_if;
import eagine.core.runtime;
import eagine.core.main_ctx;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_plane_gen.
/// @ingroup shapes
/// @see unit_plane_from
/// @see from_value_tree
/// @see unit_screen
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_plane(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) -> std::unique_ptr<generator>;

export [[nodiscard]] auto unit_plane(const vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator> {
    return unit_plane(attr_kinds, 4, 4);
}

/// @brief Tries to construct instances of unit_plane_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_plane_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

