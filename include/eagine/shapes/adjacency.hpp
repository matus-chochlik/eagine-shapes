/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_ADJACENCY_HPP
#define EAGINE_SHAPES_ADJACENCY_HPP

#include "config/basic.hpp"
#include "delegated.hpp"
#include "topology.hpp"
#include <eagine/main_ctx_object.hpp>
#include <map>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier adding triangle adjacency to drawing instructions
/// @ingroup shapes
/// @see triangle_adjacency
class triangle_adjacency_gen
  : public main_ctx_object
  , public delegated_gen {
public:
    triangle_adjacency_gen(
      std::shared_ptr<generator> gen,
      const drawing_variant var,
      main_ctx_parent parent) noexcept;

    triangle_adjacency_gen(
      std::shared_ptr<generator> gen,
      main_ctx_parent parent) noexcept
      : triangle_adjacency_gen{std::move(gen), 0, parent} {}

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool override;
    auto is_enabled(const generator_capability cap) noexcept -> bool override;

    auto index_type(const topology&) -> index_data_type;
    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const topology&) -> span_size_t;
    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    auto _topology(const drawing_variant var) noexcept -> topology&;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;

    std::map<drawing_variant, topology> _topologies;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
static inline auto add_triangle_adjacency(
  std::shared_ptr<generator> gen,
  const drawing_variant var,
  main_ctx_parent parent) noexcept {
    return std::make_unique<triangle_adjacency_gen>(
      std::move(gen), var, parent);
}
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
static inline auto add_triangle_adjacency(
  std::shared_ptr<generator> gen,
  main_ctx_parent parent) noexcept {
    return std::make_unique<triangle_adjacency_gen>(std::move(gen), parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/adjacency.inl>
#endif

#endif // EAGINE_SHAPES_ADJACENCY_HPP
