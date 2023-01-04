/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:to_json;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.main_ctx;
import :generator;
import <cstdint>;
import <iosfwd>;
import <memory>;
import <map>;

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
  main_ctx&,
  generator&,
  to_json_options& opts) noexcept -> bool;
//------------------------------------------------------------------------------
export [[nodiscard]] auto to_json(
  std::ostream&,
  generator&,
  const to_json_options&) -> std::ostream&;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

