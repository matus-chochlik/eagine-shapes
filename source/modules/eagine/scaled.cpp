/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:scaled;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :delegated;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier scaling the generated vertices by specifed amount.
/// @ingroup shapes
/// @see translate
export class scaled_gen : public delegated_gen {

public:
    scaled_gen(
      std::shared_ptr<generator> gen,
      const std::array<float, 3> s) noexcept
      : delegated_gen{std::move(gen)}
      , _s{s} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    std::array<float, 3> _s;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_gen modifier.
/// @ingroup shapes
export auto scale(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> s) noexcept {
    return std::make_unique<scaled_gen>(std::move(gen), s);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

