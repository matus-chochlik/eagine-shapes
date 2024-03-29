/// @example eagine/for_each_triangle.cpp
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
    auto& args = ctx.args();

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
    } else if(args.find("--twisted-torus")) {
        gen = shapes::unit_twisted_torus(shapes::vertex_attrib_kind::position);
    }

    if(not gen) {
        gen = shapes::unit_icosahedron(shapes::vertex_attrib_kind::position);
    }

    const auto print_info = [&](const shapes::shape_face_info& info) {
        ctx.cio()
          .print(identifier{"triangle"}, "[${a}, ${b}, ${c}]")
          .arg(identifier{"a"}, info.indices[0])
          .arg(identifier{"b"}, info.indices[1])
          .arg(identifier{"c"}, info.indices[2]);
    };

    gen->for_each_triangle({construct_from, print_info});

    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

