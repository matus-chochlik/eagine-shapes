/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_FROM_JSON_HPP
#define EAGINE_SHAPES_FROM_JSON_HPP

#include "gen_base.hpp"
#include <eagine/main_ctx_fwd.hpp>
#include <eagine/value_tree/wrappers.hpp>
#include <iosfwd>

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto from_json_stream(std::istream&, main_ctx&) noexcept
  -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/from_json.inl>
#endif

#endif // EAGINE_SHAPES_FROM_JSON_HPP
