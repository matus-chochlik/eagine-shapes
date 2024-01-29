/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.utility;
import eagine.core.runtime;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto url_shape_names() noexcept -> const auto& {
    static const std::array<string_view, 9> names{
      {"unit_cube",
       "unit_round_cube",
       "unit_plane",
       "unit_torus",
       "unit_twisted_torus",
       "unit_icosahedron",
       "unit_sphere",
       "unit_screen",
       "unit_skybox"}};
    return names;
}
//------------------------------------------------------------------------------
void for_each_shape_locator(
  string_view domain,
  callable_ref<void(string_view) noexcept> callback) noexcept {
    for(const auto path : url_shape_names()) {
        callback(std::format(
          "shape://{}/{}?view=0+position=0",
          domain.std_view(),
          path.std_view()));
    }
}
//------------------------------------------------------------------------------
auto has_shape_from(const url& locator) noexcept -> bool {
    for(const auto path : url_shape_names()) {
        if(locator.has_path(path)) {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
auto shape_from(vertex_attrib_kinds attrs, const url& locator, main_ctx& ctx)
  -> shared_holder<generator> {

    const auto q{locator.query()};
    for(const auto& info : enumerator_mapping(
          std::type_identity<shapes::vertex_attrib_kind>{}, default_selector)) {
        if(const auto var{q.arg_value_as<int>(info.name)}) {
            attrs.set(info.enumerator);
        } else if(q.arg_has_value(info.name, true)) {
            attrs.set(info.enumerator);
        }
    }
    if(auto gen{unit_cube_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_round_cube_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_plane_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_torus_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_twisted_torus_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_icosahedron_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_sphere_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{unit_screen_from(attrs, locator, ctx)}) {
        return gen;
    }
    if(auto gen{skybox_from(attrs, locator, ctx)}) {
        return gen;
    }
    return {};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
