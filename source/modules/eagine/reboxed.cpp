/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:reboxed;

import eagine.core.types;
import :generator;
import <memory>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of reboxed_gen modifier.
/// @ingroup shapes
export auto rebox(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

