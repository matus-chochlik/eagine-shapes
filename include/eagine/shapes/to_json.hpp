/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_TO_JSON_HPP
#define EAGINE_SHAPES_TO_JSON_HPP

#include "generator.hpp"
#include <eagine/logging/logger.hpp>
#include <eagine/main_ctx_fwd.hpp>
#include <eagine/value_tree/wrappers.hpp>
#include <iosfwd>
#include <map>
#include <set>

namespace eagine::shapes {
//------------------------------------------------------------------------------
struct to_json_options {
    to_json_options() noexcept = default;

    std::map<vertex_attrib_kind, std::map<std::int16_t, std::string>>
      attrib_variants;
    drawing_variant draw_variant{0};
};
//------------------------------------------------------------------------------
auto parse_from(main_ctx&, generator&, to_json_options& opts) noexcept -> bool;
//------------------------------------------------------------------------------
auto to_json(std::ostream&, generator&, const to_json_options&)
  -> std::ostream&;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/to_json.inl>
#endif

#endif // EAGINE_SHAPES_TO_JSON_HPP
