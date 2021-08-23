/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_CACHED_HPP
#define EAGINE_SHAPES_CACHED_HPP

#include "delegated.hpp"
#include <eagine/config/basic.hpp>
#include <map>
#include <mutex>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator caching the vertex attribute values, etc. if they are used.
/// @ingroup shapes
/// @see occlude
class cached_gen : public delegated_gen {

public:
    cached_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    void attrib_values(const vertex_attrib_variant, span<byte>) override;
    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<float>) override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;
    void indices(const drawing_variant, span<std::uint16_t> dest) override;
    void indices(const drawing_variant, span<std::uint32_t> dest) override;

private:
    std::mutex _mutex;
    std::map<vertex_attrib_variant, std::vector<byte>> _byte_cache;
    std::map<vertex_attrib_variant, std::vector<std::int16_t>> _int16_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint16_t>> _uint16_cache;
    std::map<vertex_attrib_variant, std::vector<std::int32_t>> _int32_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint32_t>> _uint32_cache;
    std::map<vertex_attrib_variant, std::vector<float>> _float_cache;

    std::map<drawing_variant, std::vector<std::uint8_t>> _idx8_cache;
    std::map<drawing_variant, std::vector<std::uint16_t>> _idx16_cache;
    std::map<drawing_variant, std::vector<std::uint32_t>> _idx32_cache;

    template <typename T>
    void _get_values(
      const vertex_attrib_variant,
      span<T>,
      std::map<vertex_attrib_variant, std::vector<T>>&);

    template <typename T>
    void _get_indices(
      const drawing_variant,
      span<T>,
      std::map<drawing_variant, std::vector<T>>&);
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of cached_gen modifier.
/// @ingroup shapes
static inline auto cache(std::shared_ptr<generator> gen) noexcept {
    return std::make_unique<cached_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/cached.inl>
#endif

#endif // EAGINE_SHAPES_CACHED_HPP
