/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:surface_points;

import eagine.core.types;
import eagine.core.main_ctx;
import :generator;
import std;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto surface_points(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  main_ctx_parent parent) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto surface_points(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  const vertex_attrib_variant weight_variant,
  main_ctx_parent parent) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes

