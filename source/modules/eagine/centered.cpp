/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:centered;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import :delegated;
import <memory>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of centered_gen modifier.
/// @ingroup shapes
export auto center(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

