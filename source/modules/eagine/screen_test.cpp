/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/testing/unit_begin_ctx.hpp>
import eagine.core;
import eagine.shapes;
//------------------------------------------------------------------------------
void screen_defaults_1(auto& s) {
    eagitest::case_ test{s, 1, "defaults 1"};

    auto screen{
      eagine::shapes::unit_screen(eagine::shapes::all_vertex_attrib_kinds())};

    test.ensure(bool(screen), "has generator");
    test.check(screen->vertex_count() >= 4, "vertex count");
    test.check(
      screen->has(eagine::shapes::vertex_attrib_kind::position),
      "has positions");
    test.check(
      screen->has(eagine::shapes::vertex_attrib_kind::normal), "has normals");
    test.check(
      screen->has(eagine::shapes::vertex_attrib_kind::tangent), "has tangents");
    test.check(
      screen->has(eagine::shapes::vertex_attrib_kind::bitangent),
      "has bi-tangents");
    test.check(
      screen->has(eagine::shapes::vertex_attrib_kind::face_coord),
      "has box coords");

    test.check(screen->operation_count() > 0, "has operations");
}
//------------------------------------------------------------------------------
void screen_from_url_1(auto& s) {
    eagitest::case_ test{s, 2, "from URL 1"};

    const auto p0{eagine::shapes::unit_screen_from(
      eagine::shapes::all_vertex_attrib_kinds(),
      eagine::url{"http://bad/url"},
      s.context())};

    test.check(not p0, "not p0");
}
//------------------------------------------------------------------------------
void screen_from_url_2(auto& s) {
    eagitest::case_ test{s, 3, "from URL 2"};

    const auto p1{eagine::shapes::unit_screen_from(
      eagine::shapes::all_vertex_attrib_kinds(),
      eagine::url{"shape:///unit_screen?width=1+height=1"},
      s.context())};

    test.ensure(bool(p1), "p1");
    test.check(p1->vertex_count() >= 4, "vertex count");
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
auto test_main(eagine::test_ctx& ctx) -> int {
    eagitest::ctx_suite test{ctx, "screen", 3};
    test.once(screen_defaults_1);
    test.once(screen_from_url_1);
    test.once(screen_from_url_2);
    return test.exit_code();
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    return eagine::test_main_impl(argc, argv, test_main);
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end_ctx.hpp>
