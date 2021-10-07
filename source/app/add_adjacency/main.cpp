///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/main.hpp>
#include <eagine/shapes/adjacency.hpp>
#include <eagine/shapes/from_json.hpp>
#include <eagine/shapes/to_json.hpp>
#include <iostream>

namespace eagine {

static auto get_base_generator(main_ctx& ctx)
  -> std::unique_ptr<shapes::generator> {
    // TODO: parse input file path
    return shapes::from_json_stream(std::cin, ctx);
}

auto main(main_ctx& ctx) -> int {
    using namespace eagine;

    if(auto bgen{get_base_generator(ctx)}) {
        if(auto gen{shapes::add_triangle_adjacency(std::move(bgen), ctx)}) {
            shapes::to_json_options opts;
            if(parse_from(ctx, extract(gen), opts)) {
                shapes::to_json(std::cout, extract(gen), opts) << std::endl;
            }
        }
    }

    return 0;
}

} // namespace eagine
