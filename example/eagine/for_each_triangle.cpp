/// @example eagine/for_each_triangle.cpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/console/console.hpp>
#include <eagine/main_ctx.hpp>
#include <eagine/shapes/cube.hpp>
#include <eagine/shapes/icosahedron.hpp>
#include <eagine/shapes/round_cube.hpp>
#include <eagine/shapes/sphere.hpp>
#include <eagine/shapes/torus.hpp>
#include <eagine/shapes/twisted_torus.hpp>
#include <iostream>

namespace eagine {

auto main(main_ctx& ctx) -> int {
    using namespace eagine;
    auto& args = ctx.args();

    std::shared_ptr<shapes::generator> gen;

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

    if(!gen) {
        gen = shapes::unit_icosahedron(shapes::vertex_attrib_kind::position);
    }

    const auto print_info = [&](const shapes::shape_face_info& info) {
        ctx.cio()
          .print(EAGINE_ID(triangle), "[${a}, ${b}, ${c}]")
          .arg(EAGINE_ID(a), info.indices[0])
          .arg(EAGINE_ID(b), info.indices[1])
          .arg(EAGINE_ID(c), info.indices[2]);
    };

    gen->for_each_triangle({construct_from, print_info});

    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

