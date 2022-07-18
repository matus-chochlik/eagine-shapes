/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:array;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.math;
import :delegated;
import <array>;
import <memory>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export class array_gen : public delegated_gen {
public:
    array_gen(
      std::shared_ptr<generator> gen,
      const std::array<float, 3> d,
      const span_size_t n) noexcept
      : delegated_gen{std::move(gen)}
      , _d{d}
      , _copies{n} {}

    auto vertex_count() -> span_size_t override;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    std::array<float, 3> _d;
    span_size_t _copies;

    template <typename T>
    void _indices(drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
export auto array(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> d,
  const span_size_t count) noexcept {
    return std::make_unique<array_gen>(std::move(gen), d, count);
}
//------------------------------------------------------------------------------
export auto ortho_array_xyz(
  std::shared_ptr<generator> gen,
  const std::array<float, 3> d,
  const std::array<span_size_t, 3> n) noexcept {
    const float z = 0.0F;
    return array(
      array(array(std::move(gen), {d[0], z, z}, n[0]), {z, d[1], z}, n[1]),
      {z, z, d[2]},
      n[2]);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
