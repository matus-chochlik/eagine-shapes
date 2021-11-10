/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_TO_QUADS_HPP
#define EAGINE_SHAPES_TO_QUADS_HPP

#include "config/basic.hpp"
#include "delegated.hpp"

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier transforming triangle strips to quads (lines with adjacency)
/// @ingroup shapes
/// @see to_quads
class to_quads_gen : public delegated_gen {
public:
    to_quads_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_quads_gen modifier.
/// @ingroup shapes
static inline auto to_quads(std::shared_ptr<generator> gen) noexcept {
    return std::make_unique<to_quads_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/to_quads.inl>
#endif

#endif // EAGINE_SHAPES_TO_QUADS_HPP
