/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:reboxed;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import :delegated;
import <memory>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier recalculating the vertex box coords values.
/// @ingroup shapes
/// @see rebox
export class reboxed_gen : public delegated_gen {

public:
    reboxed_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    void attrib_values(const vertex_attrib_variant, span<float>) override;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of reboxed_gen modifier.
/// @ingroup shapes
export auto rebox(std::shared_ptr<generator> gen) noexcept {
    return std::make_unique<reboxed_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

