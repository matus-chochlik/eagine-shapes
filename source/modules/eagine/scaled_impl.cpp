/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
void scaled_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    delegated_gen::attrib_values(vav, dest);

    const bool is_scaled_attrib = vav == vertex_attrib_kind::position ||
                                  vav == vertex_attrib_kind::pivot ||
                                  vav == vertex_attrib_kind::pivot_pivot ||
                                  vav == vertex_attrib_kind::vertex_pivot;

    if(is_scaled_attrib) {
        const auto m = values_per_vertex(vav);
        const auto n = vertex_count();
        for(const auto v : integer_range(n)) {
            for(const auto c : integer_range(m)) {
                dest[v * m + c] *= _s[integer(c)];
            }
        }
    }
}
//------------------------------------------------------------------------------
auto scaled_gen::bounding_sphere() -> math::sphere<float, true> {
    const auto bs = delegated_gen::bounding_sphere();
    const auto ms = math::maximum(_s[0], math::maximum(_s[1], _s[2]));
    return {bs.center(), bs.radius() * ms};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
