/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:combined;

import eagine.core.types;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export [[nodiscard]] auto combine(std::unique_ptr<generator>&& gen)
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
export template <std::size_t N>
[[nodiscard]] auto combine(std::array<std::unique_ptr<generator>, N>&& gens)
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

