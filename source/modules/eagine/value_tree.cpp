/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:value_tree;

import eagine.core.types;
import eagine.core.value_tree;
import eagine.core.main_ctx;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of value_tree_loader.
/// @ingroup shapes
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_screen
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto from_value_tree(
  valtree::compound source,
  main_ctx_parent parent) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

