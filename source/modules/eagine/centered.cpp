/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:centered;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import :delegated;
import <memory>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier recalculating the shape center from vertex positions.
/// @ingroup shapes
/// @see center
export class centered_gen : public delegated_gen {

public:
    centered_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    using delegated_gen::attrib_values;
    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto bounding_sphere() -> math::sphere<float, true> override;
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of centered_gen modifier.
/// @ingroup shapes
export auto center(std::shared_ptr<generator> gen) noexcept {
    return std::make_unique<centered_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

