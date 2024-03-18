/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes;

import std;
import eagine.core;

export import :vertex_attributes;
export import :drawing;
export import :generator;
export import :delegated;
export import :primitive_info;
export import :topology;
export import :to_json;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export [[nodiscard]] auto has_shape_from(const url& locator) noexcept -> bool;

export void for_each_shape_locator(
  string_view domain,
  callable_ref<void(string_view) noexcept> callback) noexcept;
//------------------------------------------------------------------------------
export [[nodiscard]] auto shape_from(vertex_attrib_kinds, const url&, main_ctx&)
  -> shared_holder<generator>;

export [[nodiscard]] auto shape_from(const url& locator, main_ctx& ctx)
  -> shared_holder<generator> {
    return shape_from({}, locator, ctx);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
