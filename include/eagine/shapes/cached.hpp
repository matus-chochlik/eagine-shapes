/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_SHAPES_CACHED_HPP
#define EAGINE_SHAPES_CACHED_HPP

#include "config/basic.hpp"
#include "generator.hpp"
#include <eagine/main_ctx_object.hpp>
#include <eagine/string_span.hpp>
#include <map>
#include <mutex>

namespace eagine::shapes {
//------------------------------------------------------------------------------
/// @brief Generator caching the vertex attribute values, etc. if they are used.
/// @ingroup shapes
/// @see occlude
class cached_gen
  : public main_ctx_object
  , public generator {

public:
    cached_gen(std::shared_ptr<generator> gen, main_ctx_parent parent) noexcept;

    auto attrib_kinds() noexcept -> vertex_attrib_kinds final {
        return _gen->attrib_kinds();
    }

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final {
        return _gen->enable(cap, value);
    }

    auto is_enabled(const generator_capability cap) noexcept -> bool final {
        return _gen->is_enabled(cap);
    }

    auto vertex_count() -> span_size_t final {
        return _vertex_count;
    }

    auto attribute_variants(const vertex_attrib_kind) -> span_size_t final;

    auto variant_name(const vertex_attrib_variant vav) -> string_view final;

    auto values_per_vertex(const vertex_attrib_variant) -> span_size_t final;
    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type final;
    auto is_attrib_normalized(const vertex_attrib_variant) -> bool final;

    void attrib_values(const vertex_attrib_variant, span<byte>) final;
    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>) final;
    void attrib_values(const vertex_attrib_variant, span<float>) final;

    auto draw_variant_count() -> span_size_t final {
        return _draw_variant_count;
    }

    auto index_type(const drawing_variant var) -> index_data_type final;
    auto index_count(const drawing_variant var) -> span_size_t final;

    void indices(const drawing_variant, span<std::uint8_t> dest) final;
    void indices(const drawing_variant, span<std::uint16_t> dest) final;
    void indices(const drawing_variant, span<std::uint32_t> dest) final;

    auto operation_count(const drawing_variant var) -> span_size_t final;

    void instructions(const drawing_variant, span<draw_operation> dest) final;

    auto bounding_sphere() -> math::sphere<float, true> final {
        return _bounding_sphere;
    }

private:
    const std::shared_ptr<generator> _gen;
    const span_size_t _vertex_count;
    const span_size_t _draw_variant_count;
    const math::sphere<float, true> _bounding_sphere;

    std::mutex _mutex;
    std::map<vertex_attrib_kind, span_size_t> _attrib_variants;
    std::map<vertex_attrib_variant, std::string> _variant_name;
    std::map<vertex_attrib_variant, span_size_t> _values_per_vertex;
    std::map<vertex_attrib_variant, attrib_data_type> _attrib_type;
    std::map<vertex_attrib_variant, bool> _is_normalized;

    std::map<vertex_attrib_variant, std::vector<byte>> _byte_cache;
    std::map<vertex_attrib_variant, std::vector<std::int16_t>> _int16_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint16_t>> _uint16_cache;
    std::map<vertex_attrib_variant, std::vector<std::int32_t>> _int32_cache;
    std::map<vertex_attrib_variant, std::vector<std::uint32_t>> _uint32_cache;
    std::map<vertex_attrib_variant, std::vector<float>> _float_cache;

    std::map<drawing_variant, index_data_type> _index_type;
    std::map<drawing_variant, span_size_t> _index_count;

    std::map<drawing_variant, std::vector<std::uint8_t>> _idx8_cache;
    std::map<drawing_variant, std::vector<std::uint16_t>> _idx16_cache;
    std::map<drawing_variant, std::vector<std::uint32_t>> _idx32_cache;

    std::map<drawing_variant, span_size_t> _operation_count;

    std::map<drawing_variant, std::vector<draw_operation>> _instructions;

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

    void _get_instructions(
      const drawing_variant,
      span<draw_operation>,
      std::map<drawing_variant, std::vector<draw_operation>>&);
};
//------------------------------------------------------------------------------
/// @brief Constructs instances of cached_gen modifier.
/// @ingroup shapes
static inline auto cache(
  std::shared_ptr<generator> gen,
  main_ctx_parent parent) noexcept {
    return std::make_unique<cached_gen>(std::move(gen), parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

#if !EAGINE_SHAPES_LIBRARY || defined(EAGINE_IMPLEMENTING_SHAPES_LIBRARY)
#include <eagine/shapes/cached.inl>
#endif

#endif // EAGINE_SHAPES_CACHED_HPP
