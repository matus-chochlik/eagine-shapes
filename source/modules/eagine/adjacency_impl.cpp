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
import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import eagine.core.identifier;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class triangle_adjacency_gen
  : public main_ctx_object
  , public delegated_gen {
public:
    triangle_adjacency_gen(
      shared_holder<generator> gen,
      const drawing_variant var,
      main_ctx_parent parent) noexcept;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool override;
    auto is_enabled(const generator_capability cap) noexcept -> bool override;

    auto index_type(const topology&) -> index_data_type;
    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const topology&) -> span_size_t;
    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    auto _topology(const drawing_variant var) noexcept -> topology&;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    std::map<drawing_variant, topology> _topologies;
};
//------------------------------------------------------------------------------
auto add_triangle_adjacency(
  shared_holder<generator> gen,
  const drawing_variant var,
  main_ctx_parent parent) noexcept -> shared_holder<generator> {
    return {hold<triangle_adjacency_gen>, std::move(gen), var, parent};
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::_topology(const drawing_variant var) noexcept
  -> topology& {
    auto found{find(_topologies, var)};
    if(not found) {
        topology_options opts;
        opts.features = topology_feature_bit::triangle_adjacency;
        found.emplace(
          var,
          topology{delegated_gen::base_generator(), opts, this->as_parent()});
    }
    return *found;
}
//------------------------------------------------------------------------------
triangle_adjacency_gen::triangle_adjacency_gen(
  shared_holder<generator> gen,
  const drawing_variant var,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"AjcyShpGen", parent}
  , delegated_gen{std::move(gen)} {
    enable(generator_capability::indexed_drawing, true);
    _topology(var);
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    return delegated_gen::enable(cap, value) or
           (cap == generator_capability::indexed_drawing);
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::is_enabled(const generator_capability cap) noexcept
  -> bool {
    return delegated_gen::is_enabled(cap) or
           (cap == generator_capability::indexed_drawing);
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::index_count(const topology& topo) -> span_size_t {
    return topo.triangle_count() * 6;
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::index_count(const drawing_variant var)
  -> span_size_t {
    return index_count(_topology(var));
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::index_type(const topology&) -> index_data_type {
    const auto count = delegated_gen::vertex_count();
    if(count > 65535) {
        return index_data_type::unsigned_32;
    }
    if(count > 255) {
        return index_data_type::unsigned_16;
    }
    return index_data_type::unsigned_8;
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::index_type(const drawing_variant var)
  -> index_data_type {
    return index_type(_topology(var));
}
//------------------------------------------------------------------------------
template <typename T>
void triangle_adjacency_gen::_indices(
  const drawing_variant var,
  span<T> dest) noexcept {
    auto& topo = _topology(var);
    assert(index_count(topo) <= dest.size());

    span_size_t i = 0;
    for(const auto t : integer_range(topo.triangle_count())) {
        auto& tri = topo.triangle(t);
        for(const auto v : integer_range(3)) {
            dest[i++] = limit_cast<T>(tri.vertex_index(v));
            dest[i++] = limit_cast<T>(tri.opposite_index(v));
        }
    }
    assert(i == index_count(topo));
}
//------------------------------------------------------------------------------
void triangle_adjacency_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void triangle_adjacency_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void triangle_adjacency_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
auto triangle_adjacency_gen::operation_count(const drawing_variant)
  -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
void triangle_adjacency_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    assert(not ops.empty());

    auto& topo = _topology(var);
    auto& op = ops[0];

    op = {};
    op.first = 0;
    op.count = index_count(topo);
    op.phase = 0;
    op.mode = primitive_type::triangles_adjacency;
    op.idx_type = index_type(topo);
    op.cw_face_winding = true;
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
