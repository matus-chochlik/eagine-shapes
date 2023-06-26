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
import eagine.core.identifier;
import eagine.core.math;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class reboxed_gen : public delegated_gen {

public:
    reboxed_gen(shared_holder<generator> gen) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;
};
//------------------------------------------------------------------------------
auto rebox(shared_holder<generator> gen) noexcept -> shared_holder<generator> {
    return {hold<reboxed_gen>, std::move(gen)};
}
//------------------------------------------------------------------------------
reboxed_gen::reboxed_gen(shared_holder<generator> gen) noexcept
  : delegated_gen{std::move(gen)} {
    delegated_gen::_add(vertex_attrib_kind::box_coord);
}
//------------------------------------------------------------------------------
void reboxed_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    if(vav == vertex_attrib_kind::box_coord) {

        delegated_gen::attrib_values(vertex_attrib_kind::position / 0, dest);

        std::array<float, 4> min{
          std::numeric_limits<float>::max(),
          std::numeric_limits<float>::max(),
          std::numeric_limits<float>::max(),
          std::numeric_limits<float>::max()};

        std::array<float, 4> max{
          std::numeric_limits<float>::lowest(),
          std::numeric_limits<float>::lowest(),
          std::numeric_limits<float>::lowest(),
          std::numeric_limits<float>::lowest()};

        const auto n = vertex_count();
        const auto m = values_per_vertex(vav);

        for(const auto v : integer_range(n)) {
            for(const auto c : integer_range(m)) {
                const auto k = std_size(c);

                min[k] = eagine::math::minimum(min[k], dest[v * m + c]);
                max[k] = eagine::math::maximum(max[k], dest[v * m + c]);
            }
        }

        std::array<float, 4> inorm{{}};
        for(const auto c : integer_range(m)) {
            const auto k = std_size(c);
            inorm[k] = 1.0F / (max[k] - min[k]);
        }

        for(const auto v : integer_range(n)) {
            for(const auto c : integer_range(m)) {
                const auto l = v * m + c;
                const auto k = std_size(c);
                dest[l] = (dest[l] - min[k]) * inorm[k];
            }
        }
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
