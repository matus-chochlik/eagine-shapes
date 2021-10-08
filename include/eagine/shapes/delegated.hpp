/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_DELEGATED_HPP
#define EAGINE_SHAPES_DELEGATED_HPP

#include "gen_base.hpp"
#include <eagine/config/basic.hpp>
#include <memory>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Base class for delegating shape generators based on other generators.
/// @ingroup shapes
class delegated_gen : public generator {
public:
    delegated_gen(std::shared_ptr<generator> gen) noexcept
      : _gen{std::move(gen)} {}

    auto attrib_bits() noexcept -> vertex_attrib_bits final {
        return _gen->attrib_bits() | _attr_bits;
    }

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final {
        return _gen->enable(cap, value);
    }

    auto is_enabled(const generator_capability cap) noexcept -> bool final {
        return _gen->is_enabled(cap);
    }

    auto vertex_count() -> span_size_t override {
        return _gen->vertex_count();
    }

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override {
        if(_gen->has(attrib)) {
            if(const auto count{_gen->attribute_variants(attrib)}) {
                return count;
            }
        }
        return has(attrib) ? 1 : 0;
    }

    auto variant_name(const vertex_attrib_variant vav) -> string_view override {
        return _gen->variant_name(vav);
    }

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override {
        return has_variant(vav) ? attrib_values_per_vertex(vav) : 0U;
    }

    auto attrib_type(const vertex_attrib_variant vav)
      -> attrib_data_type override {
        return _gen->attrib_type(vav);
    }

    auto is_attrib_normalized(const vertex_attrib_variant vav)
      -> bool override {
        return _gen->is_attrib_normalized(vav);
    }

    void attrib_values(const vertex_attrib_variant vav, span<byte> dest)
      override {
        _gen->attrib_values(vav, dest);
    }

    void attrib_values(const vertex_attrib_variant vav, span<std::int16_t> dest)
      override {
        _gen->attrib_values(vav, dest);
    }

    void attrib_values(const vertex_attrib_variant vav, span<std::int32_t> dest)
      override {
        _gen->attrib_values(vav, dest);
    }

    void attrib_values(
      const vertex_attrib_variant vav,
      span<std::uint16_t> dest) override {
        _gen->attrib_values(vav, dest);
    }

    void attrib_values(
      const vertex_attrib_variant vav,
      span<std::uint32_t> dest) override {
        _gen->attrib_values(vav, dest);
    }

    void attrib_values(const vertex_attrib_variant vav, span<float> dest)
      override {
        _gen->attrib_values(vav, dest);
    }

    auto draw_variant_count() -> span_size_t override {
        return _gen->draw_variant_count();
    }

    auto index_type(const drawing_variant var) -> index_data_type override {
        return _gen->index_type(var);
    }

    auto index_count(const drawing_variant var) -> span_size_t override {
        return _gen->index_count(var);
    }

    void indices(const drawing_variant var, span<std::uint8_t> dest) override {
        _gen->indices(var, dest);
    }

    void indices(const drawing_variant var, span<std::uint16_t> dest) override {
        _gen->indices(var, dest);
    }

    void indices(const drawing_variant var, span<std::uint32_t> dest) override {
        _gen->indices(var, dest);
    }

    auto operation_count(const drawing_variant var) -> span_size_t override {
        return _gen->operation_count(var);
    }

    void instructions(const drawing_variant var, span<draw_operation> ops)
      override {
        _gen->instructions(var, ops);
    }

    auto bounding_sphere() -> math::sphere<float, true> override {
        return _gen->bounding_sphere();
    }

protected:
    [[nodiscard]] auto base_generator() const noexcept
      -> std::shared_ptr<generator> {
        return _gen;
    }

    auto _add(vertex_attrib_kind attr) noexcept -> auto& {
        _attr_bits |= attr;
        return *this;
    }

private:
    std::shared_ptr<generator> _gen;
    vertex_attrib_bits _attr_bits;
};
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#endif // EAGINE_SHAPES_DELEGATED_HPP
