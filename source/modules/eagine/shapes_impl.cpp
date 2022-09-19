/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import eagine.core.types;
import eagine.core.runtime;
import eagine.core.main_ctx;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto shape_from(const url& locator, main_ctx& ctx)
  -> std::unique_ptr<generator> {

    shapes::vertex_attrib_kinds attrs;
    for(const auto& info : enumerator_mapping(
          std::type_identity<shapes::vertex_attrib_kind>{}, default_selector)) {
        if(locator.query().arg_has_value(info.name, true)) {
            attrs.set(info.enumerator);
        }
    }
    if(auto gen{unit_cube_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_torus_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_twisted_torus_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_sphere_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{skybox_from(attrs, locator, ctx)}) {
        return gen;
    }
    return {};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
