/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/input_data.hpp>
#include <eagine/main_ctx.hpp>
#include <eagine/memory/buffer.hpp>
#include <eagine/shapes/value_tree.hpp>
#include <eagine/value_tree/json.hpp>
#include <iostream>

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto from_json_stream(std::istream& input, main_ctx& ctx) noexcept
  -> std::unique_ptr<generator> {
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
} // namespace eagine::shapes
