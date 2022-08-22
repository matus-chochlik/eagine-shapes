///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#if EAGINE_SHAPES_MODULE
import eagine.core;
import eagine.shapes;
import <iostream>;
import <map>;
#else
#include <eagine/main_ctx.hpp>
#include <eagine/shapes/from_json.hpp>
#include <eagine/shapes/occluded.hpp>
#include <eagine/shapes/to_json.hpp>
#include <eagine/valid_if/positive.hpp>
#include <iostream>
#endif

namespace eagine {

static auto get_base_generator(main_ctx& ctx)
  -> std::unique_ptr<shapes::generator> {
    // TODO: parse input file path
    return shapes::from_json_stream(std::cin, ctx);
}

auto main(main_ctx& ctx) -> int {
    using namespace eagine;

    if(auto bgen{get_base_generator(ctx)}) {
        valid_if_positive<span_size_t> samples{256};
        if(const auto arg{ctx.args().find("--shape-occlusion-samples")}) {
            arg.next().parse(samples, ctx.log().error_stream());
        }
        if(auto gen{
             shapes::occlude(std::move(bgen), extract_or(samples, 256), ctx)}) {
            shapes::to_json_options opts;
            if(parse_from(ctx, extract(gen), opts)) {
                opts.attrib_variants[shapes::vertex_attrib_kind::occlusion][0];
                shapes::to_json(std::cout, extract(gen), opts) << std::endl;
            }
        }
    }

    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

