/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:to_patches;

import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_patches_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto to_patches(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

