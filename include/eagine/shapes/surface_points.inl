/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/math/functions.hpp>
#include <array>
#include <vector>

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto surface_points_gen::_topology(const drawing_variant var) noexcept
  -> ext_topology& {
    auto pos = _topologies.find(var);
    if(pos == _topologies.end()) {
        auto gen = delegated_gen::base_generator();
        pos =
          _topologies
            .emplace(
              var,
              ext_topology{
                gen, topology_feature_bit::triangle_area, this->as_parent()})
            .first;
        auto& topo = pos->second;

        topo.triangle_areas.reserve(std_size(topo.triangle_count()));
        float accumulated = 0.F;
        for(const auto t : integer_range(topo.triangle_count())) {
            accumulated += topo.triangle(t).area();
            topo.triangle_areas.push_back(accumulated);
        }
    }
    return pos->second;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto surface_points_gen::vertex_count() -> span_size_t {
    return _point_count;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void surface_points_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    const auto is_normalized_attrib = vav == vertex_attrib_kind::normal ||
                                      vav == vertex_attrib_kind::tangential ||
                                      vav == vertex_attrib_kind::bitangential;

    const auto gen = delegated_gen::base_generator();

    const auto vpv = gen->values_per_vertex(vav);
    std::vector<float> vertex_values;
    vertex_values.resize(std_size(gen->value_count(vav)));
    gen->attrib_values(vav, cover(vertex_values));

    auto& topo = _topology(0);
    EAGINE_ASSERT(!topo.triangle_areas.empty());

    std::uniform_real_distribution<float> bary_dist{0.F, 1.F};
    std::uniform_real_distribution<float> area_dist{
      0.F, topo.triangle_areas.back()};

    std::array<std::vector<float>, 4> tri_vert_values{};
    for(auto& vec : tri_vert_values) {
        vec.resize(vpv);
    }

    for(const auto p : integer_range(_point_count)) {
        const auto area_pick{area_dist(_rand)};
        const auto tri_pick_idx{std::distance(
          topo.triangle_areas.begin(),
          std::lower_bound(
            topo.triangle_areas.begin(), topo.triangle_areas.end(), area_pick))};

        const auto& tri = topo.triangle(tri_pick_idx);
        for(const auto e : integer_range(3)) {
            const auto i = tri.vertex_index(e);
            for(const auto v : integer_range(vpv)) {
                tri_vert_values[e][v] = vertex_values[i * vpv + v];
            }
        }

        const auto bary_a{bary_dist(_rand)};
        const auto bary_b{bary_dist(_rand)};
        const auto bary_0{std::min(bary_a, bary_b)};
        const auto bary_1{std::max(bary_a, bary_b) - bary_0};
        const auto bary_2{1.F - std::max(bary_a, bary_b)};

        for(const auto v : integer_range(vpv)) {
            tri_vert_values[3][v] = tri_vert_values[0][v] * bary_0 +
                                    tri_vert_values[1][v] * bary_1 +
                                    tri_vert_values[2][v] * bary_2;
        }

        float norm = 1.F;
        if(is_normalized_attrib) {
            float sum = 0.F;
            for(const auto v : integer_range(vpv)) {
                sum += std::pow(tri_vert_values[3][v], 2.F);
            }
            if(sum > 0.F) {
                norm = 1.F / std::sqrt(sum);
            }
        }

        for(const auto v : integer_range(vpv)) {
            const auto k = p * vpv + v;
            dest[k] = tri_vert_values[3][v] * norm;
        }
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto surface_points_gen::operation_count(const drawing_variant) -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void surface_points_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    EAGINE_ASSERT(ops.size() >= operation_count(var));
    EAGINE_MAYBE_UNUSED(var);

    draw_operation& op = ops[0];
    op.mode = primitive_type::points;
    op.idx_type = index_data_type::none;
    op.first = 0;
    op.count = vertex_count();
    op.primitive_restart = false;
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
