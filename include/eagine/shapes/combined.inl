/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/assert.hpp>
#include <eagine/math/functions.hpp>

namespace eagine::shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::add(std::unique_ptr<generator>&& gen) && -> combined_gen&& {
    _gens.emplace_back(std::move(gen));
    return std::move(*this);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::attrib_bits() noexcept -> vertex_attrib_bits {
    vertex_attrib_bits result{all_vertex_attrib_bits()};
    for(const auto& gen : _gens) {
        result = result & gen->attrib_bits();
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
auto combined_gen::is_enabled(const generator_capability cap) noexcept -> bool {
    for(const auto& gen : _gens) {
        if(!gen->is_enabled(cap)) {
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::vertex_count() -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        result += gen->vertex_count();
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->attribute_variants(attrib));
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    if(!_gens.empty()) {
        return _gens.front()->variant_name(vav);
    }
    return {};
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->values_per_vertex(vav));
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
auto combined_gen::is_attrib_normalized(const vertex_attrib_variant vav)
  -> bool {
    bool result = true;
    for(const auto& gen : _gens) {
        result &= gen->is_attrib_normalized(vav);
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
        EAGINE_ASSERT(gen->values_per_vertex(vav) == vpv);
        // TODO: adjust if gvpv < vpv
        offset += gvc * vpv;
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::draw_variant_count() -> span_size_t {
    span_size_t result{0};
    for(auto& gen : _gens) {
        result = math::maximum(result, gen->draw_variant_count());
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
void combined_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_8) {
        _indices(var, dest);
    } else {
        EAGINE_UNREACHABLE("Invalid function called for this index data type");
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_16) {
        _indices(var, dest);
    } else {
        EAGINE_UNREACHABLE("Invalid function called for this index data type");
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void combined_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    if(index_type(var) <= index_data_type::unsigned_32) {
        _indices(var, dest);
    } else {
        EAGINE_UNREACHABLE("Invalid function called for this index data type");
    }
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto combined_gen::operation_count(const drawing_variant var) -> span_size_t {
    span_size_t result{0};
    for(const auto& gen : _gens) {
        result += gen->operation_count(var);
    }
    return result;
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
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
EAGINE_LIB_FUNC
void combined_gen::ray_intersections(
  generator&,
  const drawing_variant var,
  const span<const math::line<float, true>> rays,
  span<optionally_valid<float>> intersections) {

    for(auto& gen : _gens) {
        gen->ray_intersections(extract(gen), var, rays, intersections);
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
