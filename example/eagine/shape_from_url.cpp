/// @example eagine/shape_from_url.cpp
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

    for(const auto& arg : args) {
        if(url locator{to_string(arg)}) {
            if(auto gen{shapes::shape_from(locator, ctx)}) {
                ctx.cio()
                  .print(
                    "shape",
                    "shape generated from ${URL} has ${count} vertices")
                  .arg("URL", locator.str())
                  .arg("count", gen->vertex_count());
            } else {
                ctx.log()
                  .error("failed to create shape from URL: ${arg}")
                  .arg("arg", arg);
            }
        } else {
            ctx.log().error("${arg} is not a valid URL").arg("arg", arg);
        }
    }
    return 0;
}

} // namespace eagine

auto main(int argc, const char** argv) -> int {
    return eagine::default_main(argc, argv, eagine::main);
}

