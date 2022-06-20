/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:surface_points;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import eagine.core.main_ctx;
import :delegated;
import :topology;
import <array>;
import <memory>;
import <map>;
import <vector>;
import <utility>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator modifier generating points on the surface of another shape.
/// @ingroup shapes
/// @see center
export class surface_points_gen
  : public main_ctx_object
  , public delegated_gen {

public:
    surface_points_gen(
      std::shared_ptr<generator> gen,
      const span_size_t point_count,
      main_ctx_parent parent) noexcept;

    surface_points_gen(
      std::shared_ptr<generator> gen,
      const span_size_t point_count,
      const vertex_attrib_variant weight_variant,
      main_ctx_parent parent) noexcept;

    auto vertex_count() -> span_size_t override;
    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    struct ext_topology : topology {
        using topology::topology;

        std::vector<std::tuple<span_size_t, std::array<float, 3>>> point_params;
    };

    auto _topology(const drawing_variant var) noexcept -> ext_topology&;

    const span_size_t _point_count{0};
    topology_options _topo_opts;

    std::map<drawing_variant, ext_topology> _topologies;
};
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export auto surface_points(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  main_ctx_parent parent) noexcept {
    return std::make_unique<surface_points_gen>(
      std::move(gen), point_count, parent);
}
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export auto surface_points(
  std::shared_ptr<generator> gen,
  const span_size_t point_count,
  const vertex_attrib_variant weight_variant,
  main_ctx_parent parent) noexcept {
    return std::make_unique<surface_points_gen>(
      std::move(gen), point_count, weight_variant, parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

