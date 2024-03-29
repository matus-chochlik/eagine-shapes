/// @example eagine/shape_topology.cpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
import eagine.core;
import eagine.shapes;
import std;

namespace eagine {

auto main(main_ctx& ctx) -> int {
    using namespace eagine;
    const auto& args = ctx.args();

    shared_holder<shapes::generator> gen;

    if(args.find("--torus")) {
        gen =
          shapes::unit_torus(shapes::vertex_attrib_kind::position, 5, 12, 0.5F);
    } else if(args.find("--sphere")) {
        gen = shapes::unit_sphere(shapes::vertex_attrib_kind::position, 5, 12);
    } else if(args.find("--cube")) {
        gen = shapes::unit_cube(shapes::vertex_attrib_kind::position);
    } else if(args.find("--round-cube")) {
        gen = shapes::unit_round_cube(shapes::vertex_attrib_kind::position, 2);
    } else if(args.find("--icosahedron")) {
        gen = shapes::unit_icosahedron(shapes::vertex_attrib_kind::position);
    }

    if(not gen) {
        gen = shapes::unit_icosahedron(shapes::vertex_attrib_kind::position);
    }

    shapes::topology_options opts;
    opts.features = shapes::all_topology_features();
    shapes::topology topo(gen, opts, ctx);

    topo.print_dot(std::cout) << std::endl;
    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

