/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:from_json;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export [[nodiscard]] auto from_json_stream(std::istream&, main_ctx&) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

