/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:scaled;

import eagine.core.types;
import :generator;
import <memory>;
import <array>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_gen modifier.
/// @ingroup shapes
export auto scale(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> s) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

