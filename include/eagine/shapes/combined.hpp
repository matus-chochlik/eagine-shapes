/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_COMBINED_HPP
#define EAGINE_SHAPES_COMBINED_HPP

#include "config/basic.hpp"
#include "generator.hpp"
#include <vector>

namespace eagine::shapes {
//------------------------------------------------------------------------------
class combined_gen : public generator {
public:
    combined_gen(std::vector<std::unique_ptr<generator>>&& gens) noexcept
      : _gens{std::move(gens)} {}

    auto add(std::unique_ptr<generator>&& gen) && -> combined_gen&&;

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final;

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final;

    auto is_enabled(const generator_capability cap) noexcept -> bool final;

    auto vertex_count() -> span_size_t override;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override;

    auto variant_name(const vertex_attrib_variant) -> string_view override;

    auto values_per_vertex(const vertex_attrib_variant) -> span_size_t override;

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override;

    auto is_attrib_normalized(const vertex_attrib_variant) -> bool override;

    void attrib_values(const vertex_attrib_variant, span<byte>) override;
    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto draw_variant_count() -> span_size_t override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) override;

private:
    std::vector<std::unique_ptr<generator>> _gens;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest);

    template <typename T>
    void _attrib_values(const vertex_attrib_variant, span<T>);
};
//------------------------------------------------------------------------------
static inline auto combine(std::unique_ptr<generator>&& gen) {
    std::vector<std::unique_ptr<generator>> v;
    v.reserve(1);
    v.emplace_back(std::move(gen));
    return std::make_unique<combined_gen>(std::move(v));
}
//------------------------------------------------------------------------------
template <std::size_t N>
static inline auto combine(std::array<std::unique_ptr<generator>, N>&& gens) {
    std::vector<std::unique_ptr<generator>> v;
    v.reserve(N);
    for(auto& gen : gens) {
        v.emplace_back(std::move(gen));
    }
    return std::make_unique<combined_gen>(std::move(v));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/combined.inl>
#endif

#endif // EAGINE_SHAPES_COMBINED_HPP
