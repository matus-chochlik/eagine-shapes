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
class cached_gen
  : public main_ctx_object
  , public generator {

public:
    cached_gen(shared_holder<generator> gen, main_ctx_parent parent) noexcept;

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final {
        return _gen->attrib_kinds();
    }

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final {
        return _gen->enable(cap, value);
    }

    auto is_enabled(const generator_capability cap) noexcept -> bool final {
        return _gen->is_enabled(cap);
    }

    auto instance_count() -> span_size_t final {
        return _instance_count;
    }

    auto vertex_count() -> span_size_t final {
        return _vertex_count;
    }

    auto attribute_variants(const vertex_attrib_kind) -> span_size_t final;

    auto variant_name(const vertex_attrib_variant vav) -> string_view final;
    auto find_variant(const vertex_attrib_kind attrib, const string_view name)
      -> vertex_attrib_variant final;

    auto values_per_vertex(const vertex_attrib_variant) -> span_size_t final;
    auto value_count(const vertex_attrib_variant vav) -> span_size_t final;
    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type final;
    auto is_attrib_integral(const vertex_attrib_variant) -> bool final;
    auto is_attrib_normalized(const vertex_attrib_variant) -> bool final;
    auto attrib_divisor(const vertex_attrib_variant) -> std::uint32_t final;

    void attrib_values(const vertex_attrib_variant, span<byte>) final;
    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<float>) final;

    auto draw_variant_count() -> span_size_t final {
        return _draw_variant_count;
    }

    auto index_type(const drawing_variant var) -> index_data_type final;
    auto index_count(const drawing_variant var) -> span_size_t final;

    void indices(const drawing_variant, span<std::uint8_t> dest) final;
    void indices(const drawing_variant, span<std::uint16_t> dest) final;
    void indices(const drawing_variant, span<std::uint32_t> dest) final;

    auto operation_count(const drawing_variant var) -> span_size_t final;

    void instructions(const drawing_variant, span<draw_operation> dest) final;

    auto bounding_sphere() -> math::sphere<float, true> final {
        return _bounding_sphere;
    }

    void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback) final;

    void random_surface_values(const random_attribute_values&) final;

    void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) final;

private:
    const shared_holder<generator> _gen;
    const span_size_t _instance_count;
    const span_size_t _vertex_count;
    const span_size_t _draw_variant_count;
    const math::sphere<float, true> _bounding_sphere;

    std::mutex _mutex;
    std::map<vertex_attrib_kind, span_size_t> _attrib_variants;
    std::map<vertex_attrib_variant, std::string> _variant_name;
    std::map<vertex_attrib_variant, span_size_t> _values_per_vertex;
    std::map<vertex_attrib_variant, std::uint32_t> _divisor;
    std::map<vertex_attrib_variant, attrib_data_type> _attrib_type;
    std::map<vertex_attrib_variant, bool> _is_integral;
    std::map<vertex_attrib_variant, bool> _is_normalized;

    std::map<vertex_attrib_variant, std::vector<byte>> _byte_cache;
    std::map<vertex_attrib_variant, std::vector<std::int16_t>> _int16_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint16_t>> _uint16_cache;
    std::map<vertex_attrib_variant, std::vector<std::int32_t>> _int32_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint32_t>> _uint32_cache;
    std::map<vertex_attrib_variant, std::vector<float>> _float_cache;

    std::map<drawing_variant, index_data_type> _index_type;
    std::map<drawing_variant, span_size_t> _index_count;

    std::map<drawing_variant, std::vector<std::uint8_t>> _idx8_cache;
    std::map<drawing_variant, std::vector<std::uint16_t>> _idx16_cache;
    std::map<drawing_variant, std::vector<std::uint32_t>> _idx32_cache;

    std::map<drawing_variant, span_size_t> _operation_count;

    std::map<drawing_variant, std::vector<draw_operation>> _instructions;

    template <typename T>
    void _get_values(
      const vertex_attrib_variant,
      span<T>,
      std::map<vertex_attrib_variant, std::vector<T>>&);

    template <typename T>
    void _get_indices(
      const drawing_variant,
      span<T>,
      std::map<drawing_variant, std::vector<T>>&);

    void _get_instructions(
      const drawing_variant,
      span<draw_operation>,
      std::map<drawing_variant, std::vector<draw_operation>>&);
};
//------------------------------------------------------------------------------
auto cache(shared_holder<generator> gen, main_ctx_parent parent) noexcept
  -> shared_holder<generator> {
    return {hold<cached_gen>, std::move(gen), parent};
}
//------------------------------------------------------------------------------
cached_gen::cached_gen(
  shared_holder<generator> gen,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"CchdShpGen", parent}
  , _gen{std::move(gen)}
  , _instance_count{_gen->instance_count()}
  , _vertex_count{_gen->vertex_count()}
  , _draw_variant_count{_gen->draw_variant_count()}
  , _bounding_sphere{_gen->bounding_sphere()} {}
//------------------------------------------------------------------------------
auto cached_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_attrib_variants, attrib)};
    if(not found) {
        found.emplace(attrib, _gen->attribute_variants(attrib));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::variant_name(const vertex_attrib_variant vav) -> string_view {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_variant_name, vav)};
    if(not found) {
        found.emplace(vav, to_string(_gen->variant_name(vav)));
    }
    return {*found};
}
//------------------------------------------------------------------------------
auto cached_gen::find_variant(
  const vertex_attrib_kind attrib,
  const string_view name) -> vertex_attrib_variant {
    return _gen->find_variant(attrib, name);
}
//------------------------------------------------------------------------------
auto cached_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_values_per_vertex, vav)};
    if(not found) {
        found.emplace(vav, _gen->values_per_vertex(vav));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::value_count(const vertex_attrib_variant vav) -> span_size_t {
    return vertex_count() * values_per_vertex(vav);
}
//------------------------------------------------------------------------------
auto cached_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_attrib_type, vav)};
    if(not found) {
        found.emplace(vav, _gen->attrib_type(vav));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::is_attrib_integral(const vertex_attrib_variant vav) -> bool {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_is_integral, vav)};
    if(not found) {
        found.emplace(vav, _gen->is_attrib_integral(vav));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::is_attrib_normalized(const vertex_attrib_variant vav) -> bool {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_is_normalized, vav)};
    if(not found) {
        found.emplace(vav, _gen->is_attrib_normalized(vav));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::attrib_divisor(const vertex_attrib_variant vav)
  -> std::uint32_t {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_divisor, vav)};
    if(not found) {
        found.emplace(vav, _gen->attrib_divisor(vav));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::index_type(const drawing_variant var) -> index_data_type {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_index_type, var)};
    if(not found) {
        found.emplace(var, _gen->index_type(var));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::index_count(const drawing_variant var) -> span_size_t {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_index_count, var)};
    if(not found) {
        found.emplace(var, _gen->index_count(var));
    }
    return *found;
}
//------------------------------------------------------------------------------
auto cached_gen::operation_count(const drawing_variant var) -> span_size_t {
    const std::lock_guard<std::mutex> lock{_mutex};
    auto found{find(_operation_count, var)};
    if(not found) {
        found.emplace(var, _gen->operation_count(var));
    }
    return *found;
}
//------------------------------------------------------------------------------
template <typename T>
void cached_gen::_get_values(
  const vertex_attrib_variant vav,
  span<T> dest,
  std::map<vertex_attrib_variant, std::vector<T>>& cache) {
    const auto& src = [this, vav, &cache]() -> const std::vector<T>& {
        const auto size = std_size(value_count(vav));
        const std::lock_guard<std::mutex> lock{_mutex};
        auto& cached = cache[vav];
        if(cached.empty()) {
            cached.resize(size);
            _gen->attrib_values(vav, cover(cached));
            log_debug("cached attribute values")
              .arg("attrib", vav.attribute())
              .arg("index", vav.index())
              .arg("size", size);
        }
        return cached;
    }();
    copy(view(src), dest);
}
//------------------------------------------------------------------------------
template <typename T>
void cached_gen::_get_indices(
  const drawing_variant var,
  span<T> dest,
  std::map<drawing_variant, std::vector<T>>& cache) {
    const auto& src = [this, var, &cache]() -> const std::vector<T>& {
        const auto size = std_size(index_count(var));
        const std::lock_guard<std::mutex> lock{_mutex};
        auto& cached = cache[var];
        if(cached.empty() and size != 0U) {
            cached.resize(size);
            _gen->indices(var, cover(cached));
            log_debug("cached vertex indices")
              .arg("variant", var)
              .arg("size", size);
        }
        return cached;
    }();
    copy(view(src), dest);
}
//------------------------------------------------------------------------------
void cached_gen::_get_instructions(
  const drawing_variant var,
  span<draw_operation> dest,
  std::map<drawing_variant, std::vector<draw_operation>>& cache) {
    const auto& src =
      [this, var, &cache]() -> const std::vector<draw_operation>& {
        const auto size = std_size(operation_count(var));
        const std::lock_guard<std::mutex> lock{_mutex};
        auto& cached = cache[var];
        if(cached.empty()) {
            cached.resize(size);
            _gen->instructions(var, cover(cached));
            log_debug("cached draw instructions")
              .arg("variant", var)
              .arg("size", size);
        }
        return cached;
    }();
    copy(view(src), dest);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _get_values(vav, dest, _byte_cache);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _get_values(vav, dest, _int16_cache);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _get_values(vav, dest, _uint16_cache);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _get_values(vav, dest, _int32_cache);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _get_values(vav, dest, _uint32_cache);
}
//------------------------------------------------------------------------------
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _get_values(vav, dest, _float_cache);
}
//------------------------------------------------------------------------------
void cached_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _get_indices(var, dest, _idx8_cache);
}
//------------------------------------------------------------------------------
void cached_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    _get_indices(var, dest, _idx16_cache);
}
//------------------------------------------------------------------------------
void cached_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    _get_indices(var, dest, _idx32_cache);
}
//------------------------------------------------------------------------------
void cached_gen::instructions(
  const drawing_variant var,
  span<draw_operation> dest) {
    _get_instructions(var, dest, _instructions);
}
//------------------------------------------------------------------------------
void cached_gen::for_each_triangle(
  generator& gen,
  const drawing_variant var,
  const callable_ref<void(const shape_face_info&)> callback) {
    return _gen->for_each_triangle(gen, var, callback);
}
//------------------------------------------------------------------------------
void cached_gen::random_surface_values(const random_attribute_values& rav) {
    _gen->random_surface_values(rav);
}
//------------------------------------------------------------------------------
void cached_gen::ray_intersections(
  generator& gen,
  const drawing_variant var,
  const span<const math::line<float, true>> rays,
  span<optionally_valid<float>> intersections) {
    _gen->ray_intersections(gen, var, rays, intersections);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
