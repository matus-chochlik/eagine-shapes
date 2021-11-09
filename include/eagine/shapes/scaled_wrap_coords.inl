/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/math/functions.hpp>
#include <limits>

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void scaled_wrap_coords_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    const bool is_scaled_wrap_coords_attrib =
      vav == vertex_attrib_kind::wrap_coord;

    if(is_scaled_wrap_coords_attrib) {

        delegated_gen::attrib_values(vav, dest);

        const span_size_t m = values_per_vertex(vav);

        for(const auto v : integer_range(vertex_count())) {
            for(const auto c : integer_range(m)) {
                dest[v * m + c] *= _scale[std_size(c)];
            }
        }
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
