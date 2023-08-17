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

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.units;
import eagine.core.math;
import eagine.core.progress;
import eagine.core.runtime;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class primitive_info_gen
  : public main_ctx_object
  , public delegated_gen {

public:
    primitive_info_gen(
      shared_holder<generator> gen,
      vertex_attrib_kinds attribs,
      main_ctx_parent parent) noexcept;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool override;
    auto is_enabled(const generator_capability cap) noexcept -> bool override;

    auto index_type(const topology&) -> index_data_type;
    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const topology&) -> span_size_t;
    auto index_count(const drawing_variant) -> span_size_t override;

    void opposite_lengths(const vertex_attrib_variant, span<float>);
    void edge_lengths(const vertex_attrib_variant, span<float>);
    void face_areas(const vertex_attrib_variant, span<float>);
    void attrib_values(const vertex_attrib_variant, span<float>) override;

private:
    auto _topology(const drawing_variant var) noexcept -> topology&;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    std::map<drawing_variant, topology> _topologies;
    vertex_attrib_kinds _attribs;
};
//------------------------------------------------------------------------------
auto add_primitive_info(
  shared_holder<generator> gen,
  vertex_attrib_kinds attribs,
  main_ctx_parent parent) noexcept -> shared_holder<generator> {
    return {hold<primitive_info_gen>, std::move(gen), attribs, parent};
}
//------------------------------------------------------------------------------
auto primitive_info_gen::_topology(const drawing_variant var) noexcept
  -> topology& {
    auto found{find(_topologies, var)};
    if(not found) {
        topology_options opts;
        opts.features = topology_feature_bit::triangle_area |
                        topology_feature_bit::edge_length;
        found.emplace(
          var,
          topology{delegated_gen::base_generator(), opts, this->as_parent()});
    }
    return *found;
}
//------------------------------------------------------------------------------
primitive_info_gen::primitive_info_gen(
  shared_holder<generator> gen,
  vertex_attrib_kinds attribs,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"PrimInfGen", parent}
  , delegated_gen{cache(std::move(gen), this->as_parent())}
  , _attribs{attribs} {
    delegated_gen::_add(_attribs);
}
//------------------------------------------------------------------------------
auto primitive_info_gen::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    return delegated_gen::enable(cap, value) or
           (cap == generator_capability::indexed_drawing);
}
//------------------------------------------------------------------------------
auto primitive_info_gen::is_enabled(const generator_capability cap) noexcept
  -> bool {
    return delegated_gen::is_enabled(cap) or
           (cap == generator_capability::indexed_drawing);
}
//------------------------------------------------------------------------------
auto primitive_info_gen::index_type(const topology&) -> index_data_type {
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
auto primitive_info_gen::index_type(const drawing_variant var)
  -> index_data_type {
    return index_type(_topology(var));
}
//------------------------------------------------------------------------------
auto primitive_info_gen::index_count(const topology& topo) -> span_size_t {
    return topo.triangle_count() * 3;
}
//------------------------------------------------------------------------------
auto primitive_info_gen::index_count(const drawing_variant var) -> span_size_t {
    return index_count(_topology(var));
}
//------------------------------------------------------------------------------
void primitive_info_gen::opposite_lengths(
  const vertex_attrib_variant,
  span<float>) {
    // TODO
}
//------------------------------------------------------------------------------
void primitive_info_gen::edge_lengths(const vertex_attrib_variant, span<float>) {
    // TODO
}
//------------------------------------------------------------------------------
void primitive_info_gen::face_areas(const vertex_attrib_variant, span<float>) {
    // TODO
}
//------------------------------------------------------------------------------
void primitive_info_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {

    if(vav == vertex_attrib_kind::opposite_length) {
        opposite_lengths(vav, dest);
    } else if(vav == vertex_attrib_kind::edge_length) {
        edge_lengths(vav, dest);
    } else if(vav == vertex_attrib_kind::face_area) {
        face_areas(vav, dest);
    } else {
        delegated_gen::attrib_values(vav, dest);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
