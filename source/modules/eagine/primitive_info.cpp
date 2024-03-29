/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:primitive_info;

import std;
import eagine.core.types;
import eagine.core.main_ctx;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of primitive_info modifier.
/// @ingroup shapes
export [[nodiscard]] auto add_primitive_info(
  shared_holder<generator> gen,
  vertex_attrib_kinds attribs,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

