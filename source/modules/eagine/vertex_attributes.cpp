/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes:vertex_attributes;

import std;
import eagine.core.types;
import eagine.core.memory;
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
    /// @brief Vertex normal vector.
    normal = 1U << 1U,
    /// @brief Vertex tangential vector.
    tangent = 1U << 2U,
    /// @brief Vertex bi-tangential vector.
    bitangent = 1U << 3U,
    /// @brief UV-texture wrapping coordinate.
    wrap_coord = 1U << 4U,
    /// @brief Generic tile coordinate.
    tile_coord = 1U << 5U,
    /// @brief Generic vertex coordinate.
    vertex_coord = 1U << 6U,
    /// @brief Vertex pivot point.
    vertex_pivot = 1U << 7U,
    /// @brief Pivot of vertex pivot point.
    pivot_pivot = 1U << 8U,
    /// @brief Shell inner vertex position.
    inner_position = 1U << 9U,
    /// @brief Vertex color value.
    color = 1U << 10U,
    /// @brief Vertex light emission value.
    emission = 1U << 11U,
    /// @brief Surface roughness value (0 - smooth, 1 - rough)
    roughness = 1U << 12U,
    /// @brief Vertex (ambient) light occlusion value (0 - occluded, 1 - exposed).
    occlusion = 1U << 13U,
    /// @brief Surface pointiness value (0 - inset, 0.5 - flat, 1 - pointy)
    pointiness = 1U << 14U,
    /// @brief Generic vertex weight value.
    weight = 1U << 15U,
    /// @brief Shape pivot point.
    pivot = 1U << 16U,
    /// @brief Normalized coordinate within shape bounding box.
    box_coord = 1U << 17U,
    /// @brief Generic face coordinate.
    face_coord = 1U << 18U,
    /// @brief Generic scalar field value.
    scalar_field = 1U << 19U,
    /// @brief Generic vector field value.
    vector_field = 1U << 20U,
    /// @brief Length of opposite edge.
    opposite_length = 1U << 21U,
    /// @brief Length of previous, next and opposite edges.
    edge_length = 1U << 22U,
    /// @brief Area of face polygon.
    face_area = 1U << 23U,
    /// @brief Instance offset value
    instance_offset = 1U << 24U,
    /// @brief Instance transformation matrix.
    instance_scale = 1U << 25U,
    /// @brief Instance scale value
    instance_transform = 1U << 26U,
    /// @brief The object id attributes (typically unique integer).
    object_id = 1U << 27U,
    /// @brief Face polygon id value (multiple faces can belong to the same polygon)
    polygon_id = 1U << 28U,
    /// @brief Face material id value.
    material_id = 1U << 29U
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
    return vertex_attrib_kinds{(1U << 30U) - 1U};
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
  math::vector<int, 2>,
  math::vector<int, 3>,
  math::vector<int, 4>,
  float,
  math::vector<float, 2>,
  math::vector<float, 3>,
  math::vector<float, 4>>;

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
/// @brief Collection of vertex_attrib_variant objects with shared pointer semantics.
/// @ingroup shapes
export class shared_vertex_attrib_variants
  : private std::shared_ptr<vertex_attrib_variant[]>
  , public memory::span<const vertex_attrib_variant> {
    using base_ptr = std::shared_ptr<vertex_attrib_variant[]>;
    using base_span = memory::span<const vertex_attrib_variant>;

public:
    shared_vertex_attrib_variants() noexcept = default;

    shared_vertex_attrib_variants(
      std::initializer_list<vertex_attrib_variant> vavs) noexcept;

    shared_vertex_attrib_variants(
      std::convertible_to<vertex_attrib_variant> auto... vavs) noexcept
        requires(sizeof...(vavs) > 0)
      : shared_vertex_attrib_variants{{vertex_attrib_variant{vavs}...}} {};

    static auto basic() noexcept -> const shared_vertex_attrib_variants&;

    auto view() const noexcept -> const base_span& {
        return *this;
    }

    auto operator==(const shared_vertex_attrib_variants& that) const noexcept
      -> bool {
        return are_equal(this->view(), that.view());
    }
};
//------------------------------------------------------------------------------
} // namespace shapes
export template <>
struct enumerator_traits<shapes::vertex_attrib_kind> {
    static auto mapping() noexcept
      -> enumerator_map_type<shapes::vertex_attrib_kind, 30>;
};
//------------------------------------------------------------------------------
namespace shapes {
/// @brief Returns the count of all vertex attribute kinds.
/// @ingroup shapes
export [[nodiscard]] constexpr auto vertex_attrib_kind_count() noexcept
  -> span_size_t {
    return enumerator_count<shapes::vertex_attrib_kind>();
}
//------------------------------------------------------------------------------
export [[nodiscard]] constexpr auto vertex_attrib_index(
  vertex_attrib_kind kind) noexcept -> span_size_t {
    return enumerator_index<shapes::vertex_attrib_kind>(kind);
}
//------------------------------------------------------------------------------
/// @brief Gets a zero-based index of a vertex attribute.
/// @ingroup shapes
export [[nodiscard]] auto vertex_attrib_index(
  const vertex_attrib_kind kind,
  span_size_t index) noexcept -> span_size_t {
    return vertex_attrib_index(kind) + index * vertex_attrib_kind_count();
}
//------------------------------------------------------------------------------
/// @brief Gets a zero-based index of a vertex attribute variant.
/// @ingroup shapes
export [[nodiscard]] auto vertex_attrib_index(
  const vertex_attrib_variant vav) noexcept -> span_size_t {
    return vertex_attrib_index(vav.attribute(), vav.index());
}
//------------------------------------------------------------------------------
export [[nodiscard]] constexpr auto vertex_attrib_by_index(
  span_size_t index) noexcept -> vertex_attrib_kind {
    return enumerator_by_index<shapes::vertex_attrib_kind>(
      index % vertex_attrib_kind_count());
}
//------------------------------------------------------------------------------
export [[nodiscard]] constexpr auto vertex_attrib_variant_by_index(
  span_size_t index) noexcept -> vertex_attrib_variant {
    return {
      enumerator_by_index<shapes::vertex_attrib_kind>(
        index % vertex_attrib_kind_count()),
      index / vertex_attrib_kind_count()};
}
//------------------------------------------------------------------------------
} // namespace shapes
//------------------------------------------------------------------------------
} // namespace eagine
