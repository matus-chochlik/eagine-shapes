/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:delegated;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import :generator;
import <cstdint>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Base class for delegating shape generators based on other generators.
/// @ingroup shapes
class delegated_gen : public generator {
public:
    delegated_gen(std::shared_ptr<generator> gen) noexcept;

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool override;

    auto is_enabled(const generator_capability cap) noexcept -> bool override;

    auto instance_count() -> span_size_t override;

    auto vertex_count() -> span_size_t override;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override;

    auto variant_name(const vertex_attrib_variant vav) -> string_view override;

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override;

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

protected:
    [[nodiscard]] auto base_generator() const noexcept
      -> std::shared_ptr<generator> {
        return _gen;
    }

    auto _add(vertex_attrib_kind attr) noexcept -> auto& {
        _attr_kinds.set(attr);
        return *this;
    }

private:
    std::shared_ptr<generator> _gen;
    vertex_attrib_kinds _attr_kinds;
};
//------------------------------------------------------------------------------
} // namespace eagine::shapes

