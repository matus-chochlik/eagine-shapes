/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:to_patches;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.math;
import :delegated;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier changing primitives in draw instructions to patches.
/// @ingroup shapes
/// @see to_patches
export class to_patches_gen : public delegated_gen {

public:
    to_patches_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    void instructions(const drawing_variant, span<draw_operation> ops) override;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_patches_gen modifier.
/// @ingroup shapes
export auto to_patches(std::shared_ptr<generator> gen) noexcept {
    return std::make_unique<to_patches_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

