
/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/testing/unit_begin.hpp>
import std;
import eagine.core;
import eagine.shapes;
//------------------------------------------------------------------------------
void vertex_attrib_kinds_all(auto& s) {
    eagitest::case_ test{s, 1, "all kinds"};
    eagitest::track trck{test, 0, 1};

    const eagine::shapes::vertex_attrib_kinds all_attrs{
      eagine::shapes::all_vertex_attrib_kinds()};

    eagine::shapes::vertex_attrib_kinds test_attrs;

    for(const auto& info :
        eagine::enumerators<eagine::shapes::vertex_attrib_kind>()) {
        test.check(all_attrs.has(info.enumerator), "has enumerator");
        test.check(test_attrs.has_not(info.enumerator), "has not enumerator");
        test_attrs.set(info.enumerator);
        trck.checkpoint(1);
    }

    test.check(all_attrs == test_attrs, "all set");
}
//------------------------------------------------------------------------------
void vertex_attrib_kinds_index(auto& s) {
    eagitest::case_ test{s, 2, "index"};
    eagitest::track trck{test, 0, 1};

    for(const auto& info :
        eagine::enumerators<eagine::shapes::vertex_attrib_kind>()) {

        test.check(
          info.enumerator ==
            eagine::shapes::vertex_attrib_by_index(
              eagine::shapes::vertex_attrib_index(info.enumerator)),
          "roundtrip");

        trck.checkpoint(1);
    }
}
//------------------------------------------------------------------------------
void shared_vertex_attrib_variants(auto& s) {
    eagitest::case_ test{s, 3, "shared variants"};

    eagine::shapes::shared_vertex_attrib_variants evavs{};

    eagine::shapes::shared_vertex_attrib_variants svavs{
      eagine::shapes::vertex_attrib_kind::position,
      eagine::shapes::vertex_attrib_variant{
        eagine::shapes::vertex_attrib_kind::color, 0},
      eagine::shapes::vertex_attrib_variant{
        eagine::shapes::vertex_attrib_kind::color, 1},
      eagine::shapes::vertex_attrib_kind::occlusion};

    test.check(svavs != evavs, "unequal to empty");
    test.check(not(svavs == evavs), "not equal to empty");

    eagine::shapes::shared_vertex_attrib_variants cvavs{svavs};

    test.check(svavs == cvavs, "equal");
    test.check(not(svavs != cvavs), "not unequal");

    test.check_equal(cvavs.size(), 4, "A");
    test.check(
      cvavs.front().attribute() == eagine::shapes::vertex_attrib_kind::position,
      "B");
    test.check(
      cvavs.back().attribute() == eagine::shapes::vertex_attrib_kind::occlusion,
      "C");
    test.check(
      cvavs[1].attribute() == eagine::shapes::vertex_attrib_kind::color, "D");
    test.check(cvavs[1].index() == 0, "E");
    test.check(
      cvavs[2].attribute() == eagine::shapes::vertex_attrib_kind::color, "F");
    test.check(cvavs[2].index() == 1, "G");
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    eagitest::suite test{argc, argv, "vertex attributes", 3};
    test.once(vertex_attrib_kinds_all);
    test.once(vertex_attrib_kinds_index);
    test.once(shared_vertex_attrib_variants);
    return test.exit_code();
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end.hpp>
