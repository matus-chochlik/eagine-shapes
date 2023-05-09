/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class translated_gen : public delegated_gen {
public:
    translated_gen(
      std::shared_ptr<generator> gen,
      const std::array<float, 3> d) noexcept
      : delegated_gen{std::move(gen)}
      , _d{d} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    std::array<float, 3> _d;
};
//------------------------------------------------------------------------------
auto translate(std::shared_ptr<generator> gen, std::array<float, 3> d) noexcept
  -> std::unique_ptr<generator> {
    return std::make_unique<translated_gen>(std::move(gen), d);
}
//------------------------------------------------------------------------------
void translated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    delegated_gen::attrib_values(vav, dest);

    const bool is_translated_attrib =
      vav == vertex_attrib_kind::position or
      vav == vertex_attrib_kind::inner_position or
      vav == vertex_attrib_kind::pivot or
      vav == vertex_attrib_kind::pivot_pivot or
      vav == vertex_attrib_kind::vertex_pivot;

    if(is_translated_attrib) {
        const auto m = values_per_vertex(vav);
        const auto n = vertex_count();
        for(const auto v : integer_range(n)) {
            for(const auto c : integer_range(m)) {
                dest[v * m + c] += _d[integer(c)];
            }
        }
    }
}
//------------------------------------------------------------------------------
auto translated_gen::bounding_sphere() -> math::sphere<float, true> {
    const auto bs = delegated_gen::bounding_sphere();
    using V = math::tvec<float, 3, true>;
    return {bs.center() + V{_d[0], _d[1], _d[2]}, bs.radius()};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
