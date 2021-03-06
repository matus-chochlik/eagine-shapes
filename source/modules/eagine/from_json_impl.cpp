/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.main_ctx;
import :generator;
import :value_tree;
import <iostream>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
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
