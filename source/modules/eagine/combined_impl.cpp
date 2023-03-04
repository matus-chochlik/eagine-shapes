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
import eagine.core.math;
import eagine.core.utility;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class combined_gen : public generator {
public:
    combined_gen(std::vector<std::unique_ptr<generator>>&& gens) noexcept;

    auto add(std::unique_ptr<generator>&& gen) && -> combined_gen&&;

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final;

    auto is_enabled(const generator_capability cap) noexcept -> bool final;

    auto instance_count() -> span_size_t final;
    auto vertex_count() -> span_size_t final;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t final;

    auto variant_name(const vertex_attrib_variant) -> string_view final;
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
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<float>) final;

    auto draw_variant_count() -> span_size_t final;

    auto index_type(const drawing_variant) -> index_data_type final;

    auto index_count(const drawing_variant) -> span_size_t final;

    void indices(const drawing_variant, span<std::uint8_t> dest) final;

    void indices(const drawing_variant, span<std::uint16_t> dest) final;

    void indices(const drawing_variant, span<std::uint32_t> dest) final;

    auto operation_count(const drawing_variant) -> span_size_t final;

    void instructions(const drawing_variant, span<draw_operation> ops) final;

    auto bounding_sphere() -> math::sphere<float, true> final;

    void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback) final;

    void random_surface_values(const random_attribute_values&) final;

    void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<std::optional<float>> intersections) final;

private:
    std::vector<std::unique_ptr<generator>> _gens;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest);

    template <typename T>
    void _attrib_values(const vertex_attrib_variant, span<T>);
};
//------------------------------------------------------------------------------
auto combine(std::unique_ptr<generator>&& gen) -> std::unique_ptr<generator> {
    std::vector<std::unique_ptr<generator>> v;
    v.reserve(1);
    v.emplace_back(std::move(gen));
    return std::make_unique<combined_gen>(std::move(v));
}
//------------------------------------------------------------------------------
template <std::size_t N>
auto combine(std::array<std::unique_ptr<generator>, N>&& gens)
  -> std::unique_ptr<generator> {
    std::vector<std::unique_ptr<generator>> v;
    v.reserve(N);
    for(auto& gen : gens) {
        v.emplace_back(std::move(gen));
    }
    return std::make_unique<combined_gen>(std::move(v));
}
//------------------------------------------------------------------------------
combined_gen::combined_gen(
  std::vector<std::unique_ptr<generator>>&& gens) noexcept
  : _gens{std::move(gens)} {}
//------------------------------------------------------------------------------
auto combined_gen::add(std::unique_ptr<generator>&& gen) && -> combined_gen&& {
    _gens.emplace_back(std::move(gen));
    return std::move(*this);
}
//------------------------------------------------------------------------------
auto combined_gen::attrib_kinds() noexcept -> vertex_attrib_kinds {
    vertex_attrib_kinds result{all_vertex_attrib_kinds()};
    for(const auto& gen : _gens) {
        result = result & gen->attrib_kinds();
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    // TODO: some sort of transation (set all or none)?
    bool result = true;
    for(const auto& gen : _gens) {
        result &= gen->enable(cap, value);
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::is_enabled(const generator_capability cap) noexcept -> bool {
    for(const auto& gen : _gens) {
        if(not gen->is_enabled(cap)) {
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
auto combined_gen::instance_count() -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        if(result == 0) {
            result = gen->instance_count();
        } else {
            assert(result == gen->instance_count());
        }
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::vertex_count() -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        result += gen->vertex_count();
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->attribute_variants(attrib));
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    for(auto& gen : _gens) {
        if(const auto found{gen->variant_name(vav)}) {
            return found;
        }
    }
    return {};
}
//------------------------------------------------------------------------------
auto combined_gen::find_variant(
  const vertex_attrib_kind attrib,
  const string_view name) -> vertex_attrib_variant {
    for(auto& gen : _gens) {
        if(const auto found{gen->find_variant(attrib, name)}) {
            return found;
        }
    }
    return {attrib, -1};
}
//------------------------------------------------------------------------------
auto combined_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->values_per_vertex(vav));
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::value_count(const vertex_attrib_variant vav) -> span_size_t {
    return vertex_count() * values_per_vertex(vav);
}
//------------------------------------------------------------------------------
auto combined_gen::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    auto result = attrib_data_type::none;
    for(const auto& gen : _gens) {
        auto temp = gen->attrib_type(vav);
        if(result == attrib_data_type::none) {
            result = temp;
        } else if(result != temp) {
            result = attrib_data_type::float_;
            break;
        }
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::is_attrib_integral(const vertex_attrib_variant vav) -> bool {
    bool result = true;
    for(const auto& gen : _gens) {
        result &= gen->is_attrib_integral(vav);
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::is_attrib_normalized(const vertex_attrib_variant vav)
  -> bool {
    bool result = true;
    for(const auto& gen : _gens) {
        result &= gen->is_attrib_normalized(vav);
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::attrib_divisor(const vertex_attrib_variant vav)
  -> std::uint32_t {
    std::uint32_t result{0U};
    for(const auto& gen : _gens) {
        if(result == 0) {
            result = gen->attrib_divisor(vav);
        } else {
            assert(result == gen->attrib_divisor(vav));
        }
    }
    return result;
}
//------------------------------------------------------------------------------
template <typename T>
void combined_gen::_attrib_values(
  const vertex_attrib_variant vav,
  span<T> dest) {
    const auto vpv = values_per_vertex(vav);
    span_size_t offset{0};
    for(const auto& gen : _gens) {
        const auto gvc = gen->vertex_count();
        auto tmp = head(skip(dest, offset), gvc * vpv);
        gen->attrib_values(vav, tmp);
        assert(gen->values_per_vertex(vav) == vpv);
        // TODO: adjust if gvpv < vpv
        offset += gvc * vpv;
    }
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
auto combined_gen::draw_variant_count() -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->draw_variant_count());
    }
    return result;
}
//------------------------------------------------------------------------------
auto combined_gen::index_type(const drawing_variant var) -> index_data_type {
    for(auto& gen : _gens) {
        if(gen->index_type(var) != index_data_type::none) {
            // TODO: smaller type if the indices fit
            return index_data_type::unsigned_32;
        }
    }
    return index_data_type::none;
}
//------------------------------------------------------------------------------
auto combined_gen::index_count(const drawing_variant var) -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        result += gen->index_count(var);
    }
    return result;
}
//------------------------------------------------------------------------------
template <typename T>
void combined_gen::_indices(const drawing_variant var, span<T> dest) {
    const auto npri = limit_cast<T>(vertex_count());
    span_size_t idx_offset{0};
    span_size_t spn_offset{0};
    for(const auto& gen : _gens) {
        const auto count = gen->index_count(var);
        const auto opri = limit_cast<T>(gen->vertex_count());
        auto temp = slice(dest, spn_offset, count);
        gen->indices(var, temp);
        for(T& idx : temp) {
            if(idx == opri) {
                idx = npri;
            } else {
                idx += idx_offset;
            }
        }
        idx_offset += opri;
        spn_offset += count;
    }
}
//------------------------------------------------------------------------------
void combined_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_8) {
        _indices(var, dest);
    } else {
        unreachable();
    }
}
//------------------------------------------------------------------------------
void combined_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_16) {
        _indices(var, dest);
    } else {
        unreachable();
    }
}
//------------------------------------------------------------------------------
void combined_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_32) {
        _indices(var, dest);
    } else {
        unreachable();
    }
}
//------------------------------------------------------------------------------
auto combined_gen::operation_count(const drawing_variant var) -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        result += gen->operation_count(var);
    }
    return result;
}
//------------------------------------------------------------------------------
void combined_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    const auto npri = limit_cast<unsigned>(vertex_count());
    const auto it = index_type(var);
    span_size_t op_offset{0};
    span_size_t idxoffset{0};
    for(auto& gen : _gens) {
        const auto opri = limit_cast<unsigned>(gen->vertex_count());
        const auto op_count = gen->operation_count();
        auto temp = slice(ops, op_offset, op_count);
        gen->instructions(var, temp);
        for(auto& op : temp) {
            if(op.idx_type != index_data_type::none) {
                op.idx_type = it;
                op.first += idxoffset;
                if(op.primitive_restart_index == opri) {
                    op.primitive_restart_index = npri;
                }
            } else {
                op.first += idxoffset;
            }
        }
        op_offset += op_count;
        idxoffset += gen->index_count();
    }
}
//------------------------------------------------------------------------------
auto combined_gen::bounding_sphere() -> math::sphere<float, true> {
    math::vector<float, 3, true> center{0.F};
    float radius{0.F};
    if(not _gens.empty()) {
        std::vector<math::sphere<float, true>> bss;
        bss.reserve(_gens.size());
        for(auto& gen : _gens) {
            bss.emplace_back(gen->bounding_sphere());
        }
        for(const auto& bs : bss) {
            center += bs.center();
        }
        center = center / float(_gens.size());
        for(const auto& bs : bss) {
            radius = math::maximum(
              radius, math::distance(center, bs.center()) + bs.radius());
        }
    }
    return {center, radius};
}
//------------------------------------------------------------------------------
void combined_gen::for_each_triangle(
  generator& gen,
  const drawing_variant var,
  const callable_ref<void(const shape_face_info&)> callback) {
    for(auto& gen : _gens) {
        gen->for_each_triangle(extract(gen), var, callback);
    }
}
//------------------------------------------------------------------------------
void combined_gen::random_surface_values(const random_attribute_values& rav) {
    for(auto& gen : _gens) {
        gen->random_surface_values(rav);
    }
}
//------------------------------------------------------------------------------
void combined_gen::ray_intersections(
  generator&,
  const drawing_variant var,
  const span<const math::line<float, true>> rays,
  span<std::optional<float>> intersections) {

    for(auto& gen : _gens) {
        gen->ray_intersections(extract(gen), var, rays, intersections);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
