/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import std;
import eagine.core;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class scaled_wrap_coords_gen : public delegated_gen {

public:
    scaled_wrap_coords_gen(
      shared_holder<generator> gen,
      std::array<float, 3> scale) noexcept
      : delegated_gen{std::move(gen)}
      , _scale{scale} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

private:
    std::array<float, 3> _scale{1.F, 1.F, 1.F};
};
//------------------------------------------------------------------------------
auto scale_wrap_coords(
  shared_holder<generator> gen,
  float x,
  float y,
  float z) noexcept -> shared_holder<generator> {
    return {
      hold<scaled_wrap_coords_gen>,
      std::move(gen),
      std::array<float, 3>{x, y, z}};
}
//------------------------------------------------------------------------------
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
                dest[v * m + c] *= _scale[integer(c)];
            }
        }
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
