/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/testing/unit_begin.hpp>
import eagine.shapes;
//------------------------------------------------------------------------------
void plane_defaults_1(auto& s) {
    eagitest::case_ test{s, 1, "defaults 1"};

    auto plane{eagine::shapes::unit_plane(
      eagine::shapes::all_vertex_attrib_kinds(), 1, 1)};

    test.ensure(bool(plane), "has generator");
    test.check(plane->vertex_count() >= 4, "vertex count");
    test.check(
      plane->has(eagine::shapes::vertex_attrib_kind::position),
      "has positions");
    test.check(
      plane->has(eagine::shapes::vertex_attrib_kind::normal), "has normals");
    test.check(
      plane->has(eagine::shapes::vertex_attrib_kind::tangent), "has tangents");
    test.check(
      plane->has(eagine::shapes::vertex_attrib_kind::bitangent),
      "has bi-tangents");
    test.check(
      plane->has(eagine::shapes::vertex_attrib_kind::box_coord),
      "has box coords");

    test.check(plane->operation_count() > 0, "has operations");
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    eagitest::suite test{argc, argv, "plane", 1};
    test.once(plane_defaults_1);
    return test.exit_code();
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end.hpp>
