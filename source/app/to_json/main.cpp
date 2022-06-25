///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/main_ctx.hpp>
#include <eagine/program_args.hpp>
#include <eagine/shapes/cube.hpp>
#include <eagine/shapes/icosahedron.hpp>
#include <eagine/shapes/round_cube.hpp>
#include <eagine/shapes/sphere.hpp>
#include <eagine/shapes/to_json.hpp>
#include <eagine/shapes/torus.hpp>
#include <eagine/shapes/twisted_torus.hpp>
#include <iostream>

namespace eagine {

static auto get_base_generator(const main_ctx& ctx) {
    const auto& args = ctx.args();

    const auto attrbits{shapes::all_vertex_attrib_kinds()};
    std::shared_ptr<shapes::generator> gen;

    if(args.find("--twisted-torus")) {
        gen = shapes::unit_twisted_torus(attrbits, 6, 48, 4, 0.5F);
    } else if(args.find("--torus")) {
        gen = shapes::unit_torus(attrbits, 5, 12, 0.5F);
    } else if(args.find("--sphere")) {
        gen = shapes::unit_sphere(attrbits, 5, 12);
    } else if(args.find("--cube")) {
        gen = shapes::unit_cube(attrbits);
    } else if(args.find("--round-cube")) {
        gen = shapes::unit_round_cube(attrbits, 2);
    } else if(args.find("--icosahedron")) {
        gen = shapes::unit_icosahedron(attrbits);
    }

    if(!gen) {
        gen = shapes::unit_icosahedron(attrbits);
    }

    return gen;
}

auto main(main_ctx& ctx) -> int {
    using namespace eagine;

    if(auto gen{get_base_generator(ctx)}) {
        shapes::to_json_options opts;
        if(parse_from(ctx, extract(gen), opts)) {
            shapes::to_json(std::cout, extract(gen), opts) << std::endl;
        }
    }
    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

