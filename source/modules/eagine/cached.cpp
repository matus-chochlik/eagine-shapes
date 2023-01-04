/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:cached;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.main_ctx;
import :generator;
import <cstdint>;
import <map>;
import <memory>;
import <mutex>;
import <vector>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of cached_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto cache(
  std::shared_ptr<generator> gen,
  main_ctx_parent parent) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

