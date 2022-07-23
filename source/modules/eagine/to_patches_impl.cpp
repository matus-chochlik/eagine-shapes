/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class to_patches_gen : public delegated_gen {

public:
    to_patches_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    void instructions(const drawing_variant, span<draw_operation> ops) override;
};
//------------------------------------------------------------------------------
auto to_patches(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator> {
    return std::make_unique<to_patches_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
void to_patches_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {

    delegated_gen::instructions(var, ops);

    for(auto& op : ops) {
        if(op.mode == primitive_type::triangles) {
            op.mode = primitive_type::patches;
            op.patch_vertices = 3;
        } else if(op.mode == primitive_type::quads) {
            op.mode = primitive_type::patches;
            op.patch_vertices = 4;
        }
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
