/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:adjacency;

import eagine.core.types;
import eagine.core.main_ctx;
import :generator;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto add_triangle_adjacency(
  std::shared_ptr<generator> gen,
  const drawing_variant var,
  main_ctx_parent parent) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto add_triangle_adjacency(
  std::shared_ptr<generator> gen,
  main_ctx_parent parent) noexcept {
    return add_triangle_adjacency(std::move(gen), 0, parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

