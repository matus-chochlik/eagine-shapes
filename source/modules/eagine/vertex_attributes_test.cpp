
/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#include <eagine/testing/unit_begin.hpp>
import eagine.core;
import eagine.shapes;
import std;
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
auto main(int argc, const char** argv) -> int {
    eagitest::suite test{argc, argv, "vertex attributes", 1};
    test.once(vertex_attrib_kinds_all);
    return test.exit_code();
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end.hpp>
