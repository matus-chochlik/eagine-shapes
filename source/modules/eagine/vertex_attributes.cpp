/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:vertex_attributes;

import std;
import eagine.core.types;
import eagine.core.math;
import eagine.core.container;
import eagine.core.reflection;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief Shape vertex attribute kind enumeration.
/// @ingroup shapes
/// @see vertex_attrib_name_and_kind
export enum class vertex_attrib_kind : std::uint32_t {
    /// @brief Vertex position.
    position = 1U << 0U,
    /// @brief Shell inner vertex position.
    inner_position = 1U << 1U,
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
    /// @brief Normalized coordinate within shape bounding box.
    box_coord = 1U << 8U,
    /// @brief UV-texture wrapping coordinate.
    wrap_coord = 1U << 9U,
    /// @brief Generic face coordinate.
    face_coord = 1U << 10U,
    /// @brief Generic tile coordinate.
    tile_coord = 1U << 11U,
    /// @brief Generic face coordinate.
    vertex_coord = 1U << 12U,
    /// @brief Vertex color value.
    color = 1U << 13U,
    /// @brief Generic vertex weight value.
    weight = 1U << 14U,
    /// @brief Vertex (ambient) light occlusion value.
    occlusion = 1U << 15U,
    /// @brief Generic scalar field value.
    scalar_field = 1U << 16U,
    /// @brief Generic vector field value.
    vector_field = 1U << 17U,
    /// @brief Length of opposite edge.
    opposite_length = 1U << 18U,
    /// @brief Length of previous, next and opposite edges.
    edge_length = 1U << 19U,
    /// @brief Area of face polygon.
    face_area = 1U << 20U,
    /// @brief Instance offset value
    instance_offset = 1U << 21U,
    /// @brief Instance scale value
    instance_scale = 1U << 22U,
    /// @brief Instance scale value
    instance_transform = 1U << 23U,
    /// @brief The object id attributes (typically unique integer).
    object_id = 1U << 24U,
    /// @brief Face polygon id value (multiple faces can belong to the same polygon)
    polygon_id = 1U << 25U,
    /// @brief Face material id value.
    material_id = 1U << 26U
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
    return vertex_attrib_kinds{(1U << 27U) - 1U};
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
/// @brief Gets a zero-based index of a vertex attribute.
/// @ingroup shapes
export [[nodiscard]] auto attrib_index(const vertex_attrib_kind attr) noexcept
  -> span_size_t;
//------------------------------------------------------------------------------
/// @brief Gets a zero-based index of a vertex attribute.
/// @ingroup shapes
export [[nodiscard]] auto attrib_index(
  const vertex_attrib_kind attr,
  span_size_t index) noexcept -> span_size_t;
//------------------------------------------------------------------------------
/// @brief Gets a zero-based index of a vertex attribute variant.
/// @ingroup shapes
export [[nodiscard]] auto attrib_index(const vertex_attrib_variant vav) noexcept
  -> span_size_t {
    return attrib_index(vav.attribute(), vav.index());
}
//------------------------------------------------------------------------------
/// @brief Gets the default number of values per vertex for an attribute kind.
/// @ingroup shapes
export [[nodiscard]] auto attrib_values_per_vertex(
  const vertex_attrib_kind attr) noexcept -> span_size_t;
//------------------------------------------------------------------------------
/// @brief Gets the default number of values per vertex for an attribute variant.
/// @ingroup shapes
export [[nodiscard]] auto attrib_values_per_vertex(
  const vertex_attrib_variant vav) noexcept {
    return attrib_values_per_vertex(vav.attribute());
}
//------------------------------------------------------------------------------
/// @brief Map from vertex attribute variant to a value of type T.
/// @ingroup shapes
export template <typename T>
class vertex_attrib_map {
public:
    /// @brief Indicates if this map is empty.
    /// @see size
    auto empty() const noexcept -> bool {
        return _map.empty();
    }

    /// @brief Returns the number of elements in this map.
    /// @see empty
    auto size() const noexcept -> span_size_t {
        return _map.size();
    }

    /// @brief Returns the number of elements in this map.
    /// @see empty
    auto clear() noexcept -> auto& {
        _map.clear();
        return *this;
    }

    /// @brief Store the specified value associated to the attribute variant.
    auto set(vertex_attrib_variant vav, T&& value) -> auto& {
        _map[vav] = std::move(value);
        return *this;
    }

    /// @brief Get the specified value associated to the attribute variant.
    auto get(vertex_attrib_variant vav) const noexcept
      -> optional_reference<const T> {
        return eagine::find(_map, vav);
    }

    auto begin() const noexcept {
        return _map.begin();
    }

    auto end() const noexcept {
        return _map.end();
    }

private:
    flat_map<vertex_attrib_variant, T> _map;
};
//------------------------------------------------------------------------------
using vertex_attrib_value_variant = std::variant<
  std::monostate,
  int,
  math::vector<int, 2, true>,
  math::vector<int, 3, true>,
  math::vector<int, 4, true>,
  float,
  math::vector<float, 2, true>,
  math::vector<float, 3, true>,
  math::vector<float, 4, true>>;

/// @brief Type for storing of vertex attribute values.
/// @ingroup shapes
export class vertex_attrib_value : public vertex_attrib_value_variant {
    using base = vertex_attrib_value_variant;

public:
    using base::base;

    constexpr explicit operator bool() const noexcept {
        return not std::holds_alternative<std::monostate>(*this);
    }
};
//------------------------------------------------------------------------------
/// @brief Gets the default value per vertex for an attribute kind.
/// @ingroup shapes
export [[nodiscard]] auto default_attrib_value(
  const vertex_attrib_kind attr) noexcept -> vertex_attrib_value;
//------------------------------------------------------------------------------
/// @brief Gets the default value per vertex for an attribute variant.
/// @ingroup shapes
export [[nodiscard]] auto default_attrib_value(
  const vertex_attrib_variant vav) noexcept {
    return default_attrib_value(vav.attribute());
}
//------------------------------------------------------------------------------
} // namespace shapes
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<shapes::vertex_attrib_kind>,
  const Selector) noexcept {
    using shapes::vertex_attrib_kind;
    return enumerator_map_type<vertex_attrib_kind, 27>{
      {{"position", vertex_attrib_kind::position},
       {"inner_position", vertex_attrib_kind::inner_position},
       {"normal", vertex_attrib_kind::normal},
       {"tangent", vertex_attrib_kind::tangent},
       {"bitangent", vertex_attrib_kind::bitangent},
       {"pivot", vertex_attrib_kind::pivot},
       {"pivot_pivot", vertex_attrib_kind::pivot_pivot},
       {"vertex_pivot", vertex_attrib_kind::vertex_pivot},
       {"box_coord", vertex_attrib_kind::box_coord},
       {"wrap_coord", vertex_attrib_kind::wrap_coord},
       {"face_coord", vertex_attrib_kind::face_coord},
       {"tile_coord", vertex_attrib_kind::tile_coord},
       {"vertex_coord", vertex_attrib_kind::vertex_coord},
       {"color", vertex_attrib_kind::color},
       {"weight", vertex_attrib_kind::weight},
       {"occlusion", vertex_attrib_kind::occlusion},
       {"scalar_field", vertex_attrib_kind::scalar_field},
       {"vector_field", vertex_attrib_kind::vector_field},
       {"opposite_length", vertex_attrib_kind::opposite_length},
       {"edge_length", vertex_attrib_kind::edge_length},
       {"face_area", vertex_attrib_kind::face_area},
       {"instance_offset", vertex_attrib_kind::instance_offset},
       {"instance_scale", vertex_attrib_kind::instance_scale},
       {"instance_transform", vertex_attrib_kind::instance_transform},
       {"object_id", vertex_attrib_kind::object_id},
       {"polygon_id", vertex_attrib_kind::polygon_id},
       {"material_id", vertex_attrib_kind::material_id}}};
}
//------------------------------------------------------------------------------
} // namespace eagine
