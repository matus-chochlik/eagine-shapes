/// @example eagine/for_each_triangle.cpp
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/program_args.hpp>
#include <eagine/shapes/cube.hpp>
#include <eagine/shapes/icosahedron.hpp>
#include <eagine/shapes/round_cube.hpp>
#include <eagine/shapes/sphere.hpp>
#include <eagine/shapes/torus.hpp>
#include <eagine/shapes/twisted_torus.hpp>
#include <iostream>

auto main(int argc, const char** argv) -> int {
    using namespace eagine;
    const program_args args(argc, argv);

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

    const auto print_info = [](const shapes::shape_face_info& info) {
        std::cout << '[';
        std::cout << info.indices[0] << ", ";
        std::cout << info.indices[1] << ", ";
        std::cout << info.indices[2] << "]\n";
    };

    gen->for_each_triangle({construct_from, print_info});

    return 0;
}