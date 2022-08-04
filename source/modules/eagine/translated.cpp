/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:translated;

import eagine.core.types;
import :generator;
import <array>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of translated_gen modifier.
/// @ingroup shapes
export auto translate(
  std::shared_ptr<generator> gen,
  std::array<float, 3> d) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

