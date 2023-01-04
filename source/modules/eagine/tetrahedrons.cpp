/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:tetrahedrons;

import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of marching_tetrahedrons_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_torus
/// @see unit_screen
/// @see unit_twisted_torus
export [[nodiscard]] auto marching_tetrahedrons(vertex_attrib_kinds attr_kinds)
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

