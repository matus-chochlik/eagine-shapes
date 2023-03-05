/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes;

import eagine.core.runtime;
import eagine.core.main_ctx;

export import :vertex_attributes;
export import :drawing;
export import :generator;
export import :delegated;
export import :primitive_info;
export import :topology;
export import :to_json;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export [[nodiscard]] auto shape_from(vertex_attrib_kinds, const url&, main_ctx&)
  -> std::unique_ptr<generator>;

export [[nodiscard]] auto shape_from(const url& locator, main_ctx& ctx)
  -> std::unique_ptr<generator> {
    return shape_from({}, locator, ctx);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
