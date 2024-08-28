/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import std;
import eagine.core;

namespace eagine::shapes {
//------------------------------------------------------------------------------
struct topology_data {
    unsigned coords_per_vertex{0U};
    unsigned weights_per_vertex{0U};
    std::vector<float> vertex_positions;
    std::vector<float> vertex_weights;
    std::vector<unsigned> indices;
    std::vector<draw_operation> operations;

    auto values_of(const unsigned i) const noexcept {
        assert(coords_per_vertex > 0);
        return head(
          skip(view(vertex_positions), span_size(coords_per_vertex * i)),
          span_size(coords_per_vertex));
    }

    auto to_vec(const span<const float> v) const noexcept {
        return math::vector<float, 3>{v[0], v[1], v[2]};
    }

    auto vec_of(const unsigned i) const noexcept {
        return to_vec(values_of(i));
    }

    auto distance_delta(const mesh_triangle& tri) const noexcept -> float {
        const auto a = vec_of(tri.vertex_index(0));
        const auto b = vec_of(tri.vertex_index(1));
        const auto c = vec_of(tri.vertex_index(2));
        return 0.1F *
               std::min(
                 std::min(distance(a, b), distance(b, c)), distance(a, c));
    }

    auto have_same_position(
      const unsigned i,
      const unsigned j,
      const float delta) const noexcept -> bool {
        const auto p = vec_of(i);
        const auto q = vec_of(j);
        return distance(p, q) < delta;
    }

    auto is_same_vertex(unsigned i, unsigned j, float delta) const noexcept
      -> bool {
        return (i == j) or have_same_position(i, j, delta);
    }
};
//------------------------------------------------------------------------------
auto mesh_triangle::setup_adjacent(
  mesh_triangle& r,
  const topology_data& topo) noexcept
  -> std::tuple<bool, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t> {
    auto& l = *this;
    assert(l.index() != r.index());
    const auto delta = topo.distance_delta(l);
    for(const auto i : integer_range(std_size(3))) {
        for(const auto j : integer_range(std_size(3))) {
            if(topo.is_same_vertex(l.curri(i), r.curri(j), delta)) {
                if(topo.is_same_vertex(l.previ(i), r.previ(j), delta)) {
                    l._adjacent[prevv(i)] = &r;
                    l._opposite[prevv(i)] = narrow(nextv(j));
                    r._adjacent[prevv(j)] = &l;
                    r._opposite[prevv(j)] = narrow(nextv(i));
                    return {
                      true,
                      narrow(prevv(i)),
                      narrow(i),
                      narrow(prevv(j)),
                      narrow(j)};
                } else if(topo.is_same_vertex(l.previ(i), r.nexti(j), delta)) {
                    l._adjacent[prevv(i)] = &r;
                    l._opposite[prevv(i)] = narrow(prevv(j));
                    r._adjacent[j] = &l;
                    r._opposite[j] = narrow(nextv(i));
                    return {
                      true,
                      narrow(prevv(i)),
                      narrow(i),
                      narrow(j),
                      narrow(nextv(j))};
                } else if(topo.is_same_vertex(l.nexti(i), r.previ(j), delta)) {
                    l._adjacent[i] = &r;
                    l._opposite[i] = narrow(nextv(j));
                    r._adjacent[prevv(j)] = &l;
                    r._opposite[prevv(j)] = narrow(prevv(i));
                    return {
                      true,
                      narrow(i),
                      narrow(nextv(i)),
                      narrow(prevv(j)),
                      narrow(j)};
                } else if(topo.is_same_vertex(l.nexti(i), r.nexti(j), delta)) {
                    l._adjacent[i] = &r;
                    l._opposite[i] = narrow(prevv(j));
                    r._adjacent[j] = &l;
                    r._opposite[j] = narrow(prevv(i));
                    return {
                      true,
                      narrow(i),
                      narrow(nextv(i)),
                      narrow(j),
                      narrow(nextv(j))};
                }
            }
        }
    }
    return {false, 0U, 0U, 0U, 0U};
}
//------------------------------------------------------------------------------
auto topology::print_dot(std::ostream& out) const -> std::ostream& {
    out << "graph MeshTopology {\n";
    out << "overlap=voronoi;\n";
    out << "node [shape=triangle];\n";

    for(const auto& tri : _triangles) {
        const auto i = tri.index();
        out << "t" << i << " [label=\"" << i << "\"];\n";
    }

    out << "node [shape=point];\n";
    for(const auto& [idxt, edg] : _edges) {
        auto [lidx, ridx] = idxt;

        out << "et" << lidx << "t" << ridx << ";\n";

        for(const auto t : integer_range(std_size(2))) {
            auto [bi, ei] = edg.edge_vertices(signedness_cast(t));
            auto& tri = edg.triangle(signedness_cast(t));
            out << "et" << lidx << "t" << ridx << " -- "
                << "t" << tri.index() << "[label=\"<" << bi << "," << ei
                << ">\\n[" << tri.vertex_index(bi) << ","
                << tri.vertex_index(ei) << "]"
                << "\"];\n";
        }
    }
    out << "}";
    return out;
}
//------------------------------------------------------------------------------
void topology::_scan_topology(topology_options opts) {
    const drawing_variant var = _gen->draw_variant(opts.draw_variant_index);
    topology_data data;

    data.coords_per_vertex =
      limit_cast<unsigned>(_gen->values_per_vertex(opts.position_variant));
    data.vertex_positions.resize(
      integer(_gen->value_count(opts.position_variant)));
    _gen->attrib_values(opts.position_variant, cover(data.vertex_positions));

    if(_gen->values_per_vertex(opts.weight_variant) < 1) {
        opts.features.clear(topology_feature_bit::triangle_weight);
    }

    if(opts.features.has(topology_feature_bit::triangle_weight)) {
        data.weights_per_vertex =
          limit_cast<unsigned>(_gen->values_per_vertex(opts.weight_variant));
        data.vertex_weights.resize(
          integer(_gen->value_count(opts.weight_variant)));
        _gen->attrib_values(opts.weight_variant, cover(data.vertex_weights));
    }

    data.indices.resize(integer(_gen->index_count(var)));
    _gen->indices(var, cover(data.indices));

    data.operations.resize(integer(_gen->operation_count(var)));
    _gen->instructions(var, cover(data.operations));

    auto scan_ops = progress().activity(
      "processing shape draw operations", integer(data.operations.size()));

    for(auto& operation : data.operations) {
        const bool indexed = operation.idx_type != index_data_type::none;
        span_size_t i;

        const auto is_pri{[&]() {
            return indexed and
                   data.indices[i] == operation.primitive_restart_index;
        }};

        const auto add_triangle{[&](int a, int b, int c) {
            if(indexed) {
                _triangles.emplace_back(
                  _triangles.size(),
                  data.indices[integer(operation.first + i + a)],
                  data.indices[integer(operation.first + i + b)],
                  data.indices[integer(operation.first + i + c)]);
            } else {
                _triangles.emplace_back(
                  _triangles.size(),
                  to_index(operation.first + i + a),
                  to_index(operation.first + i + b),
                  to_index(operation.first + i + c));
            }

            auto& tri = _triangles.back();
            if(opts.features.has(topology_feature_bit::triangle_area)) {
                assert(data.coords_per_vertex >= 3U);
                const auto vpv = data.coords_per_vertex;
                const auto& pos = data.vertex_positions;
                const auto ia = tri.vertex_index(0);
                const auto ib = tri.vertex_index(1);
                const auto ic = tri.vertex_index(2);
                tri.set_area(math::triangle<float>{
                  math::vector<float, 3>{
                    pos[ia * vpv + 0], pos[ia * vpv + 1], pos[ia * vpv + 2]},
                  math::vector<float, 3>{
                    pos[ib * vpv + 0], pos[ib * vpv + 1], pos[ib * vpv + 2]},
                  math::vector<float, 3>{
                    pos[ic * vpv + 0], pos[ic * vpv + 1], pos[ic * vpv + 2]}}
                               .area());
            }
            if(opts.features.has(topology_feature_bit::triangle_weight)) {
                assert(data.coords_per_vertex >= 1U);
                const auto vpv = data.weights_per_vertex;
                const auto& wgt = data.vertex_weights;
                const auto ia = tri.vertex_index(0);
                const auto ib = tri.vertex_index(1);
                const auto ic = tri.vertex_index(2);
                tri.set_weight(wgt[ia * vpv] + wgt[ib * vpv] + wgt[ic * vpv]);
            }
        }};

        if(operation.mode == primitive_type::triangles) {
            for(i = 0; i < operation.count; i += 3) {
                if(operation.cw_face_winding) {
                    add_triangle(0, 1, 2);
                } else {
                    add_triangle(0, 2, 1);
                }
            }
        } else if(operation.mode == primitive_type::triangle_strip) {
            for(i = 2; i < operation.count; i += 2) {
                if(is_pri()) {
                    ++i;
                    continue;
                }
                if(operation.cw_face_winding) {
                    add_triangle(-2, -1, 0);
                    add_triangle(0, -1, 1);
                } else {
                    add_triangle(-1, -2, 0);
                    add_triangle(-1, 0, 1);
                }
            }
        }
        scan_ops.advance_progress();
    }
    scan_ops.finish();

    if(opts.features.has(topology_feature_bit::triangle_adjacency)) {
        const auto scan_tris = progress().activity(
          "processing shape triangles", integer(_triangles.size()));

        for(auto& ltri : _triangles) {
            const auto lidx = ltri.index();
            for(auto& rtri : _triangles) {
                const auto ridx = rtri.index();
                if(lidx < ridx) {
                    const auto key = std::make_tuple(
                      std::min(lidx, ridx), std::max(lidx, ridx));
                    if(not _edges.contains(key)) {
                        auto [should_add, leb, lee, reb, ree] =
                          ltri.setup_adjacent(rtri, data);
                        if(should_add) {
                            _edges.emplace(
                              key, mesh_edge{ltri, leb, lee, rtri, reb, ree});
                        }
                    }
                }
            }
            scan_tris.advance_progress();
        }
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
