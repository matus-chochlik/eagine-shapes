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
void generator_capabilities_all(auto& s) {
    eagitest::case_ test{s, 1, "all"};
    eagitest::track trck{test, 0, 1};

    const eagine::shapes::generator_capabilities all_caps{
      eagine::shapes::all_generator_capabilities()};

    eagine::shapes::generator_capabilities test_caps;

    for(const auto& info :
        eagine::enumerators<eagine::shapes::generator_capability>()) {
        test.check(all_caps.has(info.enumerator), "has enumerator");
        test.check(test_caps.has_not(info.enumerator), "has not enumerator");
        test_caps.set(info.enumerator);
        trck.checkpoint(1);
    }

    test.check(all_caps == test_caps, "all set");
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    eagitest::suite test{argc, argv, "generator capabilities", 1};
    test.once(generator_capabilities_all);
    return test.exit_code();
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end.hpp>
