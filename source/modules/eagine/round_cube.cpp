/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:round_cube;

import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_round_cube_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export auto unit_round_cube(
  const vertex_attrib_kinds attr_kinds,
  const int divisions) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
export auto unit_round_cube(const vertex_attrib_kinds attr_kinds) {
    return unit_round_cube(attr_kinds, 8);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

