/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:to_quads;

import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_quads_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto to_quads(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

