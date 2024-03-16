
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
auto main(int argc, const char** argv) -> int {
    eagitest::suite test{argc, argv, "vertex attributes", 2};
    test.once(vertex_attrib_kinds_all);
    test.once(vertex_attrib_kinds_index);
    return test.exit_code();
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end.hpp>
