/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:scaled_wrap_coords;

import eagine.core.types;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_wrap_coords_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto scale_wrap_coords(
  std::shared_ptr<generator> gen,
  float x,
  float y,
  float z) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

