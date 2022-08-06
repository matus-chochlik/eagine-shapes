/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:occluded;

import eagine.core.types;
import eagine.core.main_ctx;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of occluded_gen modifier.
/// @ingroup shapes
export auto occlude(
  std::shared_ptr<generator> gen,
  const span_size_t samples,
  main_ctx_parent parent) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

