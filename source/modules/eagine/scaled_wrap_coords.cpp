/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:scaled_wrap_coords;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :delegated;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier scaling the shape wrap coordinates by specified amount.
/// @ingroup shapes
/// @see center
export class scaled_wrap_coords_gen : public delegated_gen {

public:
    scaled_wrap_coords_gen(
      std::shared_ptr<generator> gen,
      std::array<float, 3> scale) noexcept
      : delegated_gen{std::move(gen)}
      , _scale{scale} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

private:
    std::array<float, 3> _scale{1.F, 1.F, 1.F};
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_wrap_coords_gen modifier.
/// @ingroup shapes
export auto scale_wrap_coords(
  std::shared_ptr<generator> gen,
  float x,
  float y,
  float z) noexcept {
    return std::make_unique<scaled_wrap_coords_gen>(
      std::move(gen), std::array<float, 3>{x, y, z});
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

