/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.units;
import eagine.core.math;
import eagine.core.main_ctx;
import <array>;
import <algorithm>;
import <map>;
import <random>;
import <vector>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
surface_points_gen::surface_points_gen(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"SurfPtsGen", parent}
  , delegated_gen{std::move(gen)}
  , _point_count{point_count} {
    _topo_opts.features.set(topology_feature_bit::triangle_area);
}
//------------------------------------------------------------------------------
surface_points_gen::surface_points_gen(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  const vertex_attrib_variant weight_variant,
  main_ctx_parent parent) noexcept
  : surface_points_gen{std::move(gen), point_count, parent} {
    _topo_opts.features.set(topology_feature_bit::triangle_weight);
    _topo_opts.weight_variant = weight_variant;
}
//------------------------------------------------------------------------------
auto surface_points_gen::_topology(const drawing_variant var) noexcept
  -> ext_topology& {
    auto pos = _topologies.find(var);
    if(pos == _topologies.end()) {
        auto gen = delegated_gen::base_generator();
        pos = _topologies
                .emplace(var, ext_topology{gen, _topo_opts, this->as_parent()})
                .first;
        auto& topo = pos->second;

        std::vector<float> triangle_areas;
        triangle_areas.reserve(std_size(topo.triangle_count()));
        float accumulated = 0.F;
        for(const auto t : integer_range(topo.triangle_count())) {
            const auto& tri = topo.triangle(t);
            accumulated += tri.area() * tri.weight();
            triangle_areas.push_back(accumulated);
        }

        std::default_random_engine rand{std::random_device{}()};
        std::uniform_real_distribution<float> bary_dist{0.F, 1.F};
        std::uniform_real_distribution<float> area_dist{
          0.F, triangle_areas.back()};

        topo.point_params.reserve(std_size(_point_count));

        for([[maybe_unused]] const auto p : integer_range(_point_count)) {
            const auto area_pick{area_dist(rand)};
            const auto tri_pick_idx{std::distance(
              triangle_areas.begin(),
              std::lower_bound(
                triangle_areas.begin(), triangle_areas.end(), area_pick))};

            const auto bary_a{bary_dist(rand)};
            const auto bary_b{bary_dist(rand)};
            const auto bary_0{std::min(bary_a, bary_b)};
            const auto bary_1{std::max(bary_a, bary_b) - bary_0};
            const auto bary_2{1.F - std::max(bary_a, bary_b)};

            topo.point_params.push_back(
              {tri_pick_idx, {bary_0, bary_1, bary_2}});
        }
    }
    return pos->second;
}
//------------------------------------------------------------------------------
auto surface_points_gen::vertex_count() -> span_size_t {
    return _point_count;
}
//------------------------------------------------------------------------------
void surface_points_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    const auto is_normalized_attrib = vav == vertex_attrib_kind::normal ||
                                      vav == vertex_attrib_kind::tangent ||
                                      vav == vertex_attrib_kind::bitangent;

    const auto gen = delegated_gen::base_generator();

    const auto vpv = gen->values_per_vertex(vav);
    std::vector<float> vertex_values;
    vertex_values.resize(integer(gen->value_count(vav)));
    gen->attrib_values(vav, cover(vertex_values));

    auto& topo = _topology(0);

    std::array<std::vector<float>, 4> tri_vert_values{};
    for(auto& vec : tri_vert_values) {
        vec.resize(vpv);
    }

    span_size_t k = 0;
    for(const auto& [idx, bary] : topo.point_params) {
        const auto& tri = topo.triangle(idx);
        for(const auto e : integer_range(3)) {
            const auto i = tri.vertex_index(e);
            for(const auto v : integer_range(vpv)) {
                tri_vert_values[e][v] = vertex_values[i * vpv + v];
            }
        }

        for(const auto v : integer_range(vpv)) {
            tri_vert_values[3][v] = tri_vert_values[0][v] * bary[0] +
                                    tri_vert_values[1][v] * bary[1] +
                                    tri_vert_values[2][v] * bary[2];
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
            dest[k++] = tri_vert_values[3][v] * norm;
        }
    }
    assert(k == _point_count * vpv);
}
//------------------------------------------------------------------------------
auto surface_points_gen::operation_count(const drawing_variant) -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
void surface_points_gen::instructions(
  [[maybe_unused]] const drawing_variant var,
  span<draw_operation> ops) {
    assert(ops.size() >= operation_count(var));

    draw_operation& op = ops[0];
    op.mode = primitive_type::points;
    op.idx_type = index_data_type::none;
    op.first = 0;
    op.count = vertex_count();
    op.primitive_restart = false;
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
