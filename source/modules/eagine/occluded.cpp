/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:occluded;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import eagine.core.main_ctx;
import :delegated;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier calculating vertex occlusion weights.
/// @ingroup shapes
/// @see occlude
export class occluded_gen
  : public main_ctx_object
  , public delegated_gen {

public:
    occluded_gen(
      std::shared_ptr<generator> gen,
      const span_size_t samples,
      main_ctx_parent parent) noexcept;

    void occlusions(const vertex_attrib_variant, span<float>);
    void attrib_values(const vertex_attrib_variant, span<float>) override;

private:
    span_size_t _samples{64};
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of occluded_gen modifier.
/// @ingroup shapes
export auto occlude(
  std::shared_ptr<generator> gen,
  const span_size_t samples,
  main_ctx_parent parent) noexcept {
    return std::make_unique<occluded_gen>(std::move(gen), samples, parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

