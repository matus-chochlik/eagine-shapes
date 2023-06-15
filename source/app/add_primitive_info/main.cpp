///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
import eagine.core;
import eagine.shapes;
import <iostream>;
import <map>;

namespace eagine {

static auto get_base_generator(main_ctx& ctx)
  -> std::unique_ptr<shapes::generator> {
    // TODO: parse input file path
    return shapes::from_json_stream(std::cin, ctx);
}

auto main(main_ctx& ctx) -> int {
    using namespace eagine;

    if(auto bgen{get_base_generator(ctx)}) {
        shapes::vertex_attrib_kinds kinds;
        // TODO: other kinds
        kinds.set(shapes::vertex_attrib_kind::opposite_length);
        if(auto gen{shapes::add_primitive_info(std::move(bgen), kinds, ctx)}) {
            shapes::to_json_options opts;
            if(parse_from(ctx, *gen, opts)) {
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

