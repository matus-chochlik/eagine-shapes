/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:vertex_attributes;

import eagine.core.types;
import eagine.core.reflection;
import <array>;
import <cstdint>;
import <type_traits>;
import <tuple>;
import <utility>;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief Shape vertex attribute kind enumeration.
/// @ingroup shapes
/// @see vertex_attrib_name_and_kind
export enum class vertex_attrib_kind : std::uint32_t {
    /// @brief The object id attributes (typically unique integer).
    object_id = 1U << 0U,
    /// @brief Vertex position.
    position = 1U << 1U,
    /// @brief Vertex normal vector.
    normal = 1U << 2U,
    /// @brief Vertex tangential vector.
    tangent = 1U << 3U,
    /// @brief Vertex bi-tangential vector.
    bitangent = 1U << 4U,
    /// @brief Shape pivot point.
    pivot = 1U << 5U,
    /// @brief Pivot of vertex pivot point.
    pivot_pivot = 1U << 6U,
    /// @brief Vertex pivot point.
    vertex_pivot = 1U << 7U,
    /// @brief Length of opposite edge.
    opposite_length = 1U << 8U,
    /// @brief Length of previous, next and opposite edges.
    edge_length = 1U << 9U,
    /// @brief Area of face polygon.
    face_area = 1U << 10U,
    /// @brief Normalized coordinate within shape bounding box.
    box_coord = 1U << 11U,
    /// @brief UV-texture wrapping coordinate.
    wrap_coord = 1U << 12U,
    /// @brief Generic face coordinate.
    face_coord = 1U << 13U,
    /// @brief Generic tile coordinate.
    tile_coord = 1U << 14U,
    /// @brief Generic face coordinate.
    vertex_coord = 1U << 15U,
    /// @brief Vertex color value.
    color = 1U << 16U,
    /// @brief Generic vertex weight value.
    weight = 1U << 17U,
    /// @brief Vertex (ambient) light occlusion value.
    occlusion = 1U << 18U,
    /// @brief Generic scalar field value.
    scalar_field = 1U << 19U,
    /// @brief Generic vector field value.
    vector_field = 1U << 20U,
    /// @brief Instance offset value
    instance_offset = 1U << 21U,
    /// @brief Instance scale value
    instance_scale = 1U << 22U,
    /// @brief Instance scale value
    instance_transform = 1U << 23U,
    /// @brief Face polygon id value (multiple faces can belong to the same polygon)
    polygon_id = 1U << 24U,
    /// @brief Face material id value.
    material_id = 1U << 25U
    // also fix all_vertex_attrib_kinds
};
//------------------------------------------------------------------------------
/// @brief Alias for attribute named and kind pair.
/// @ingroup shapes
export using vertex_attrib_name_and_kind =
  name_and_enumerator<vertex_attrib_kind>;
//------------------------------------------------------------------------------
/// @brief Alias for vertex_attrib_kind bitfield type.
/// @ingroup shapes
export using vertex_attrib_kinds = bitfield<vertex_attrib_kind>;
//------------------------------------------------------------------------------
/// @brief Returns vertex_attrib_kinds value with all bits set.
/// @ingroup shapes
export constexpr auto all_vertex_attrib_kinds() noexcept
  -> vertex_attrib_kinds {
    return vertex_attrib_kinds{(1U << 26U) - 1U};
}
//------------------------------------------------------------------------------
/// @brief Bitwise-or operator for vertex_attrib_kind bits.
/// @ingroup shapes
export constexpr auto operator|(
  const vertex_attrib_kind a,
  const vertex_attrib_kind b) noexcept -> vertex_attrib_kinds {
    return {a, b};
}
//------------------------------------------------------------------------------
/// @brief Class designating an vertex attribute variant in a shape generator.
/// @ingroup shapes
/// @see vertex_attrib_variants
export class vertex_attrib_variant {
public:
    /// @brief Default constructor.
    constexpr vertex_attrib_variant() noexcept = default;

    /// @brief Construction from vertex attribute kind enumerator.
    constexpr vertex_attrib_variant(const vertex_attrib_kind a) noexcept
      : _attrib{a} {}

    /// @brief Construction from vertex attribute kind enumerator and index.
    constexpr vertex_attrib_variant(
      const vertex_attrib_kind a,
      const span_size_t v) noexcept
      : _attrib{a}
      , _index{std::int16_t(v)} {}

    /// @brief Construction from vertex attribute kind and another attribute variant.
    constexpr vertex_attrib_variant(
      const vertex_attrib_kind a,
      const vertex_attrib_variant vav) noexcept
      : _attrib{a}
      , _index{vav._index} {}

    /// @brief Return the attribute kind.
    [[nodiscard]] constexpr auto attribute() const noexcept
      -> vertex_attrib_kind {
        return _attrib;
    }

    /// @brief Check if the stored index is valid.
    [[nodiscard]] constexpr auto has_valid_index() const noexcept {
        return _index >= 0;
    }

    /// @brief Check if the stored index is valid.
    /// @see has_valid_index
    [[nodiscard]] constexpr explicit operator bool() const noexcept {
        return has_valid_index();
    }

    /// @brief Returns the index of the attribute variant.
    [[nodiscard]] constexpr auto index() const noexcept -> span_size_t {
        return _index;
    }

    [[nodiscard]] constexpr operator std::array<const vertex_attrib_variant, 1>()
      const noexcept {
        return {{*this}};
    }

    [[nodiscard]] constexpr auto as_tuple() const noexcept {
        return std::make_tuple(_attrib, _index);
    }

    /// @brief Equality comparison.
    [[nodiscard]] friend constexpr auto operator==(
      const vertex_attrib_variant l,
      const vertex_attrib_variant r) noexcept {
        return l.as_tuple() == r.as_tuple();
    }

    /// @brief Nonequality comparison.
    [[nodiscard]] friend constexpr auto operator!=(
      const vertex_attrib_variant l,
      const vertex_attrib_variant r) noexcept {
        return l.as_tuple() != r.as_tuple();
    }

    /// @brief Less-than comparison.
    [[nodiscard]] friend constexpr auto operator<(
      const vertex_attrib_variant l,
      const vertex_attrib_variant r) noexcept {
        return l.as_tuple() < r.as_tuple();
    }

    /// @brief Variant and kind equality comparison.
    [[nodiscard]] friend constexpr auto operator==(
      const vertex_attrib_variant l,
      const vertex_attrib_kind r) noexcept {
        return l._attrib == r;
    }

    /// @brief Variant and kind nonequality comparison.
    [[nodiscard]] friend constexpr auto operator!=(
      const vertex_attrib_variant l,
      const vertex_attrib_kind r) noexcept {
        return l._attrib != r;
    }

private:
    vertex_attrib_kind _attrib{};
    std::int16_t _index{0};
};
//------------------------------------------------------------------------------
/// @brief Operator for constructing of vertex_attrib_variant from kind and index.
/// @ingroup shapes
export [[nodiscard]] constexpr auto operator/(
  const vertex_attrib_kind attrib,
  const span_size_t variant_index) noexcept -> vertex_attrib_variant {
    return {attrib, variant_index};
}
//------------------------------------------------------------------------------
/// @brief Array of several vertex attribute variant instances.
/// @ingroup shapes
/// @see vertex_attrib_variant
export template <std::size_t N>
using vertex_attrib_variants = std::array<const vertex_attrib_variant, N>;
//------------------------------------------------------------------------------
/// @brief Operator for creating of single element array of vertex attrib variant.
/// @ingroup shapes
/// @see vertex_attrib_variant
/// @see vertex_attrib_variants
export [[nodiscard]] constexpr auto operator+(
  const vertex_attrib_variant a) noexcept -> vertex_attrib_variants<1> {
    return {{a}};
}
//------------------------------------------------------------------------------
/// @brief Operator for creating of two element array of vertex attrib variants.
/// @ingroup shapes
/// @see vertex_attrib_variant
/// @see vertex_attrib_variants
export [[nodiscard]] constexpr auto operator+(
  const vertex_attrib_variant a,
  const vertex_attrib_variant b) noexcept -> vertex_attrib_variants<2> {
    return {{a, b}};
}
//------------------------------------------------------------------------------
// append_attrib
export template <std::size_t N, std::size_t... I>
[[nodiscard]] constexpr auto do_append_attrib(
  const vertex_attrib_variants<N>& a,
  const vertex_attrib_variant b,
  const std::index_sequence<I...>) noexcept {
    return vertex_attrib_variants<N + 1>{{a[I]..., b}};
}
//------------------------------------------------------------------------------
/// @brief Operator for concatenation of vertex attrib variants into an array.
/// @ingroup shapes
/// @see vertex_attrib_variant
/// @see vertex_attrib_variants
export template <std::size_t N>
[[nodiscard]] constexpr auto operator+(
  const vertex_attrib_variants<N>& a,
  const vertex_attrib_variant b) noexcept -> vertex_attrib_variants<N + 1> {
    return do_append_attrib(a, b, std::make_index_sequence<N>());
}
//------------------------------------------------------------------------------
/// @brief Extracts vertex attribute kind bits from and vertex attribute variants array.
/// @ingroup shapes
/// @see vertex_attrib_kinds
export template <std::size_t N>
[[nodiscard]] auto get_attrib_kinds(
  const vertex_attrib_variants<N>& vaals) noexcept -> vertex_attrib_kinds {
    vertex_attrib_kinds res;

    for(const vertex_attrib_variant& vaal : vaals) {
        res |= vaal.attribute();
    }

    return res;
}
//------------------------------------------------------------------------------
/// @brief Gets the default number of values per vertex for an attribute kind.
/// @ingroup shapes
export [[nodiscard]] auto attrib_values_per_vertex(
  const vertex_attrib_kind attr) noexcept -> span_size_t {
    switch(attr) {
        case vertex_attrib_kind::color:
            return 4;
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::normal:
        case vertex_attrib_kind::tangent:
        case vertex_attrib_kind::bitangent:
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::vertex_pivot:
        case vertex_attrib_kind::edge_length:
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::vertex_coord:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::instance_offset:
        case vertex_attrib_kind::vector_field:
            return 3;
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::tile_coord:
            return 2;
        case vertex_attrib_kind::opposite_length:
        case vertex_attrib_kind::face_area:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::scalar_field:
        case vertex_attrib_kind::occlusion:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
        case vertex_attrib_kind::instance_scale:
            return 1;
        case vertex_attrib_kind::instance_transform:
            return 16;
    }
    return 0;
}
//------------------------------------------------------------------------------
/// @brief Gets the default number of values per vertex for an attribute variant.
/// @ingroup shapes
export [[nodiscard]] auto attrib_values_per_vertex(
  const vertex_attrib_variant vav) noexcept {
    return attrib_values_per_vertex(vav.attribute());
}
//------------------------------------------------------------------------------
} // namespace shapes
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<shapes::vertex_attrib_kind>,
  const Selector) noexcept {
    using shapes::vertex_attrib_kind;
    return enumerator_map_type<vertex_attrib_kind, 26>{
      {{"object_id", vertex_attrib_kind::object_id},
       {"position", vertex_attrib_kind::position},
       {"normal", vertex_attrib_kind::normal},
       {"tangent", vertex_attrib_kind::tangent},
       {"bitangent", vertex_attrib_kind::bitangent},
       {"pivot", vertex_attrib_kind::pivot},
       {"pivot_pivot", vertex_attrib_kind::pivot_pivot},
       {"vertex_pivot", vertex_attrib_kind::vertex_pivot},
       {"opposite_length", vertex_attrib_kind::opposite_length},
       {"edge_length", vertex_attrib_kind::edge_length},
       {"face_area", vertex_attrib_kind::face_area},
       {"box_coord", vertex_attrib_kind::box_coord},
       {"wrap_coord", vertex_attrib_kind::wrap_coord},
       {"face_coord", vertex_attrib_kind::face_coord},
       {"tile_coord", vertex_attrib_kind::tile_coord},
       {"vertex_coord", vertex_attrib_kind::vertex_coord},
       {"color", vertex_attrib_kind::color},
       {"weight", vertex_attrib_kind::weight},
       {"scalar_field", vertex_attrib_kind::scalar_field},
       {"vector_field", vertex_attrib_kind::vector_field},
       {"occlusion", vertex_attrib_kind::occlusion},
       {"instance_offset", vertex_attrib_kind::instance_offset},
       {"instance_scale", vertex_attrib_kind::instance_scale},
       {"instance_transform", vertex_attrib_kind::instance_transform},
       {"polygon_id", vertex_attrib_kind::polygon_id},
       {"material_id", vertex_attrib_kind::material_id}}};
}
//------------------------------------------------------------------------------
} // namespace eagine
