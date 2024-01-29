/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/testing/unit_begin_ctx.hpp>
import eagine.core;
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
void plane_from_url_1(auto& s) {
    eagitest::case_ test{s, 2, "from URL 1"};

    const auto p0{eagine::shapes::unit_plane_from(
      eagine::shapes::all_vertex_attrib_kinds(),
      eagine::url{"http://bad/url"},
      s.context())};

    test.check(not p0, "not p0");
}
//------------------------------------------------------------------------------
void plane_from_url_2(auto& s) {
    eagitest::case_ test{s, 3, "from URL 2"};

    const auto p1{eagine::shapes::unit_plane_from(
      eagine::shapes::all_vertex_attrib_kinds(),
      eagine::url{"shape:///unit_plane?width=1+height=1"},
      s.context())};

    test.ensure(bool(p1), "p1");
    test.check(p1->vertex_count() >= 4, "vertex count");
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
auto test_main(eagine::test_ctx& ctx) -> int {
    eagitest::ctx_suite test{ctx, "plane", 3};
    test.once(plane_defaults_1);
    test.once(plane_from_url_1);
    test.once(plane_from_url_2);
    return test.exit_code();
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    return eagine::test_main_impl(argc, argv, test_main);
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end_ctx.hpp>
