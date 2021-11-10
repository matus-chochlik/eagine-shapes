/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_OCCLUDED_HPP
#define EAGINE_SHAPES_OCCLUDED_HPP

#include "config/basic.hpp"
#include "delegated.hpp"
#include <eagine/main_ctx_object.hpp>
#include <utility>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier calculating vertex occlusion weights.
/// @ingroup shapes
/// @see occlude
class occluded_gen
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
static inline auto occlude(
  std::shared_ptr<generator> gen,
  const span_size_t samples,
  main_ctx_parent parent) noexcept {
    return std::make_unique<occluded_gen>(std::move(gen), samples, parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/occluded.inl>
#endif

#endif // EAGINE_SHAPES_OCCLUDED_HPP
