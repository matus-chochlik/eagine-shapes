/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:translated;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :delegated;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier translating the generated vertices by specifed amount.
/// @ingroup shapes
/// @see translate
export class translated_gen : public delegated_gen {
public:
    translated_gen(
      std::shared_ptr<generator> gen,
      const std::array<float, 3> d) noexcept
      : delegated_gen{std::move(gen)}
      , _d{d} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    std::array<float, 3> _d;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of translated_gen modifier.
/// @ingroup shapes
export auto translate(
  std::shared_ptr<generator> gen,
  std::array<float, 3> d) noexcept {
    return std::make_unique<translated_gen>(std::move(gen), d);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

