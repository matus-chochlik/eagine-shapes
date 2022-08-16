/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:array;

import eagine.core.types;
import :generator;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export auto array(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> d,
  const span_size_t count) noexcept -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
export auto ortho_array_xyz(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> d,
  const std::array<span_size_t, 3> n) noexcept {
    const float z = 0.0F;
    return array(
      array(array(std::move(gen), {d[0], z, z}, n[0]), {z, d[1], z}, n[1]),
      {z, z, d[2]},
      n[2]);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
