///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
import eagine.core;
import eagine.shapes;
import <iostream>;
import <map>;

namespace eagine {

static auto get_base_generator(main_ctx& ctx)
  -> shared_holder<shapes::generator> {
    // TODO: parse input file path
    return shapes::from_json_stream(std::cin, ctx);
}

auto main(main_ctx& ctx) -> int {
    using namespace eagine;

    if(auto bgen{get_base_generator(ctx)}) {
        valid_if_positive<span_size_t> samples{256};
        if(const auto arg{ctx.args().find("--shape-occlusion-samples")}) {
            assign_if_fits(arg.next(), samples);
        }
        if(auto gen{
             shapes::occlude(std::move(bgen), samples.value_or(256), ctx)}) {
            shapes::to_json_options opts;
            if(parse_from(ctx, *gen, opts)) {
                opts.attrib_variants[shapes::vertex_attrib_kind::occlusion][0];
                shapes::to_json(std::cout, *gen, opts) << std::endl;
            }
        }
    }

    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

