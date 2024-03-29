/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:to_json;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.runtime;
import eagine.core.math;
import eagine.core.main_ctx;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export struct to_json_options {
    to_json_options() noexcept = default;

    std::map<vertex_attrib_kind, std::map<std::int16_t, std::string>>
      attrib_variants;
    drawing_variant draw_variant{0};
};
//------------------------------------------------------------------------------
export [[nodiscard]] auto parse_from(
  const url&,
  generator&,
  to_json_options& opts) noexcept -> bool;

export [[nodiscard]] auto parse_from(
  main_ctx&,
  generator&,
  to_json_options& opts) noexcept -> bool;
//------------------------------------------------------------------------------
export auto to_json(std::ostream&, generator&, const to_json_options&)
  -> std::ostream&;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

