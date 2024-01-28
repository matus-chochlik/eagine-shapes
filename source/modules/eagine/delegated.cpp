/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:delegated;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.utility;
import eagine.core.valid_if;
import eagine.core.math;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Base class for delegating shape generators based on other generators.
/// @ingroup shapes
class delegated_gen : public generator {
public:
    delegated_gen(shared_holder<generator> gen) noexcept
      : _gen{std::move(gen)} {}

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool override;

    auto is_enabled(const generator_capability cap) noexcept -> bool override;

    auto instance_count() -> span_size_t override;

    auto vertex_count() -> span_size_t override;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override;

    auto variant_name(const vertex_attrib_variant vav) -> string_view override;

    auto find_variant(const vertex_attrib_kind attrib, const string_view name)
      -> vertex_attrib_variant override;

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override;

    auto value_count(const vertex_attrib_variant vav) -> span_size_t override;

    auto attrib_type(const vertex_attrib_variant vav)
      -> attrib_data_type override;

    auto is_attrib_integral(const vertex_attrib_variant vav) -> bool override;

    auto is_attrib_normalized(const vertex_attrib_variant vav) -> bool override;

    auto attrib_divisor(const vertex_attrib_variant vav)
      -> std::uint32_t override;

    void attrib_values(const vertex_attrib_variant vav, span<byte> dest)
      override;

    void attrib_values(const vertex_attrib_variant vav, span<std::int16_t> dest)
      override;

    void attrib_values(const vertex_attrib_variant vav, span<std::int32_t> dest)
      override;

    void attrib_values(
      const vertex_attrib_variant vav,
      span<std::uint16_t> dest) override;

    void attrib_values(
      const vertex_attrib_variant vav,
      span<std::uint32_t> dest) override;

    void attrib_values(const vertex_attrib_variant vav, span<float> dest)
      override;

    auto draw_variant_count() -> span_size_t override;

    auto index_type(const drawing_variant var) -> index_data_type override;

    auto index_count(const drawing_variant var) -> span_size_t override;

    void indices(const drawing_variant var, span<std::uint8_t> dest) override;

    void indices(const drawing_variant var, span<std::uint16_t> dest) override;

    void indices(const drawing_variant var, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant var) -> span_size_t override;

    void instructions(const drawing_variant var, span<draw_operation> ops)
      override;

    auto bounding_sphere() -> math::sphere<float, true> override;

    void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback) override;

    void random_surface_values(const random_attribute_values&) override;

    void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) override;

protected:
    [[nodiscard]] auto base_generator() const noexcept
      -> shared_holder<generator> {
        return _gen;
    }

    auto _add(vertex_attrib_kinds attrs) noexcept -> auto& {
        _attr_kinds.merge(attrs);
        return *this;
    }

private:
    shared_holder<generator> _gen;
    vertex_attrib_kinds _attr_kinds;
};
//------------------------------------------------------------------------------
inline auto delegated_gen::attrib_kinds() noexcept -> vertex_attrib_kinds {
    return _gen->attrib_kinds() | _attr_kinds;
}
//------------------------------------------------------------------------------
inline auto delegated_gen::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    return _gen->enable(cap, value);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::is_enabled(const generator_capability cap) noexcept
  -> bool {
    return _gen->is_enabled(cap);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::instance_count() -> span_size_t {
    return _gen->instance_count();
}
//------------------------------------------------------------------------------
inline auto delegated_gen::vertex_count() -> span_size_t {
    return _gen->vertex_count();
}
//------------------------------------------------------------------------------
inline auto delegated_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    if(_gen->has(attrib)) {
        if(const auto count{_gen->attribute_variants(attrib)}) {
            return count;
        }
    }
    return has(attrib) ? 1 : 0;
}
//------------------------------------------------------------------------------
inline auto delegated_gen::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    return _gen->variant_name(vav);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::find_variant(
  const vertex_attrib_kind attrib,
  const string_view name) -> vertex_attrib_variant {
    return _gen->find_variant(attrib, name);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    return has_variant(vav) ? attrib_values_per_vertex(vav) : 0U;
}
//------------------------------------------------------------------------------
inline auto delegated_gen::value_count(const vertex_attrib_variant vav)
  -> span_size_t {
    return vertex_count() * values_per_vertex(vav);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    return _gen->attrib_type(vav);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::is_attrib_integral(const vertex_attrib_variant vav)
  -> bool {
    return _gen->is_attrib_integral(vav);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::is_attrib_normalized(const vertex_attrib_variant vav)
  -> bool {
    return _gen->is_attrib_normalized(vav);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::attrib_divisor(const vertex_attrib_variant vav)
  -> std::uint32_t {
    return _gen->attrib_divisor(vav);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _gen->attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::draw_variant_count() -> span_size_t {
    return _gen->draw_variant_count();
}
//------------------------------------------------------------------------------
inline auto delegated_gen::index_type(const drawing_variant var)
  -> index_data_type {
    return _gen->index_type(var);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::index_count(const drawing_variant var)
  -> span_size_t {
    return _gen->index_count(var);
}
//------------------------------------------------------------------------------
inline void delegated_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
inline void delegated_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _gen->indices(var, dest);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::operation_count(const drawing_variant var)
  -> span_size_t {
    return _gen->operation_count(var);
}
//------------------------------------------------------------------------------
inline void delegated_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    _gen->instructions(var, ops);
}
//------------------------------------------------------------------------------
inline auto delegated_gen::bounding_sphere() -> math::sphere<float, true> {
    return _gen->bounding_sphere();
}
//------------------------------------------------------------------------------
inline void delegated_gen::for_each_triangle(
  generator& gen,
  const drawing_variant var,
  const callable_ref<void(const shape_face_info&)> callback) {
    return _gen->for_each_triangle(gen, var, callback);
}
//------------------------------------------------------------------------------
inline void delegated_gen::random_surface_values(
  const random_attribute_values& rav) {
    _gen->random_surface_values(rav);
}
//------------------------------------------------------------------------------
inline void delegated_gen::ray_intersections(
  generator& gen,
  const drawing_variant var,
  const span<const math::line<float, true>> rays,
  span<optionally_valid<float>> intersections) {
    _gen->ray_intersections(gen, var, rays, intersections);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

