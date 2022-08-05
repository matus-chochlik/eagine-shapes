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
import eagine.core.math;

namespace eagine::shapes {
//------------------------------------------------------------------------------
// delegated_generator
//------------------------------------------------------------------------------
delegated_gen::delegated_gen(std::shared_ptr<generator> gen) noexcept
  : _gen{std::move(gen)} {}
//------------------------------------------------------------------------------
auto delegated_gen::attrib_kinds() noexcept -> vertex_attrib_kinds {
    return _gen->attrib_kinds() | _attr_kinds;
}
//------------------------------------------------------------------------------
auto delegated_gen::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    return _gen->enable(cap, value);
}
//------------------------------------------------------------------------------
auto delegated_gen::is_enabled(const generator_capability cap) noexcept
  -> bool {
    return _gen->is_enabled(cap);
}
//------------------------------------------------------------------------------
auto delegated_gen::instance_count() -> span_size_t {
    return _gen->instance_count();
}
//------------------------------------------------------------------------------
auto delegated_gen::vertex_count() -> span_size_t {
    return _gen->vertex_count();
}
//------------------------------------------------------------------------------
auto delegated_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    if(_gen->has(attrib)) {
        if(const auto count{_gen->attribute_variants(attrib)}) {
            return count;
        }
    }
    return has(attrib) ? 1 : 0;
}
//------------------------------------------------------------------------------
auto delegated_gen::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    return _gen->variant_name(vav);
}
//------------------------------------------------------------------------------
auto delegated_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    return has_variant(vav) ? attrib_values_per_vertex(vav) : 0U;
}
//------------------------------------------------------------------------------
auto delegated_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    return _gen->attrib_type(vav);
}
//------------------------------------------------------------------------------
auto delegated_gen::is_attrib_integral(const vertex_attrib_variant vav)
  -> bool {
    return _gen->is_attrib_integral(vav);
}
//------------------------------------------------------------------------------
auto delegated_gen::is_attrib_normalized(const vertex_attrib_variant vav)
  -> bool {
    return _gen->is_attrib_normalized(vav);
}
//------------------------------------------------------------------------------
auto delegated_gen::attrib_divisor(const vertex_attrib_variant vav)
  -> std::uint32_t {
    return _gen->attrib_divisor(vav);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
auto delegated_gen::draw_variant_count() -> span_size_t {
    return _gen->draw_variant_count();
}
//------------------------------------------------------------------------------
auto delegated_gen::index_type(const drawing_variant var) -> index_data_type {
    return _gen->index_type(var);
}
//------------------------------------------------------------------------------
auto delegated_gen::index_count(const drawing_variant var) -> span_size_t {
    return _gen->index_count(var);
}
//------------------------------------------------------------------------------
void delegated_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
void delegated_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
auto delegated_gen::operation_count(const drawing_variant var) -> span_size_t {
    return _gen->operation_count(var);
}
//------------------------------------------------------------------------------
void delegated_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    _gen->instructions(var, ops);
}
//------------------------------------------------------------------------------
auto delegated_gen::bounding_sphere() -> math::sphere<float, true> {
    return _gen->bounding_sphere();
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

