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
class scaled_gen : public delegated_gen {

public:
    scaled_gen(
      shared_holder<generator> gen,
      const std::array<float, 3> s) noexcept
      : delegated_gen{std::move(gen)}
      , _s{s} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto bounding_sphere() -> math::sphere<float> override;

private:
    std::array<float, 3> _s;
};
//------------------------------------------------------------------------------
auto scale(shared_holder<generator> gen, const std::array<float, 3> s) noexcept
  -> shared_holder<generator> {
    return {hold<scaled_gen>, std::move(gen), s};
}
//------------------------------------------------------------------------------
void scaled_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    delegated_gen::attrib_values(vav, dest);

    const bool is_scaled_attrib = vav == vertex_attrib_kind::position or
                                  vav == vertex_attrib_kind::inner_position or
                                  vav == vertex_attrib_kind::pivot or
                                  vav == vertex_attrib_kind::pivot_pivot or
                                  vav == vertex_attrib_kind::vertex_pivot or
                                  vav == vertex_attrib_kind::face_area or
                                  vav == vertex_attrib_kind::edge_length or
                                  vav == vertex_attrib_kind::opposite_length;

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
auto scaled_gen::bounding_sphere() -> math::sphere<float> {
    const auto bs = delegated_gen::bounding_sphere();
    const auto ms = math::maximum(_s[0], math::maximum(_s[1], _s[2]));
    return {bs.center(), bs.radius() * ms};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
