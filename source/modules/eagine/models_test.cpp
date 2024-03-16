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
void model_cube(auto& s) {
    eagitest::case_ test{s, 1, "defaults 1"};
    using eagine::shapes::vertex_attrib_kind;

    auto model{eagine::shapes::model_cube(s.context())};

    test.ensure(bool(model), "has generator");
    test.check(model->vertex_count() == 30, "vertex count");
    const auto check_attr{
      [&](vertex_attrib_kind attr, eagine::string_view label) {
          test.check(model->has(attr), label);
          std::vector<float> values;
          values.resize(model->value_count(attr));
          model->attrib_values(attr, eagine::cover(values));
      }};
    check_attr(vertex_attrib_kind::position, "has position");
    check_attr(vertex_attrib_kind::normal, "has normal");
    check_attr(vertex_attrib_kind::tangent, "has tangent");
    check_attr(vertex_attrib_kind::bitangent, "has bitangent");
    check_attr(vertex_attrib_kind::wrap_coord, "has wrap coord");
    check_attr(vertex_attrib_kind::color, "has color");
    check_attr(vertex_attrib_kind::pointiness, "has pointiness");
    check_attr(vertex_attrib_kind::roughness, "has roughness");
    check_attr(vertex_attrib_kind::occlusion, "has occlusion");

    test.check(model->operation_count() > 0, "has operations");
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
auto test_main(eagine::test_ctx& ctx) -> int {
    eagitest::ctx_suite test{ctx, "models", 1};
    test.once(model_cube);
    return test.exit_code();
}
//------------------------------------------------------------------------------
auto main(int argc, const char** argv) -> int {
    return eagine::test_main_impl(argc, argv, test_main);
}
//------------------------------------------------------------------------------
#include <eagine/testing/unit_end_ctx.hpp>
