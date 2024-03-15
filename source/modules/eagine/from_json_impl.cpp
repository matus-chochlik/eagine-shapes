/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import std;
import eagine.core;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto from_json_stream(std::istream& input, main_ctx& ctx) noexcept
  -> shared_holder<generator> {
    memory::buffer json_data;
    if(read_stream_data(input, json_data)) {
        if(const auto json_text{as_chars(view(json_data))}) {
            if(auto tree{valtree::from_json_text(json_text, ctx)}) {
                return shapes::from_value_tree(std::move(tree), ctx);
            } else {
                ctx.log().error("failed to parse JSON input");
            }
        } else {
            ctx.log().error("loaded empty JSON text");
        }
    } else {
        ctx.log().error("failed to read stream data");
    }
    return {};
}
//------------------------------------------------------------------------------
// from resource
//------------------------------------------------------------------------------
auto from_json_resource(
  const embedded_resource& resource,
  main_ctx& ctx) noexcept -> shared_holder<generator> {
    return from_value_tree(
      valtree::from_json_text(as_chars(resource.unpack(ctx)), ctx), ctx);
}
//------------------------------------------------------------------------------
auto model_cube(main_ctx& ctx) noexcept -> shared_holder<generator> {
    return from_json_resource(embedded<"Cube">(), ctx);
}
//------------------------------------------------------------------------------
auto model_cube_from(const url& locator, main_ctx& ctx) noexcept
  -> shared_holder<generator> {
    if(locator.has_path("/model_cube")) {
        return model_cube(ctx);
    }
    return {};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
