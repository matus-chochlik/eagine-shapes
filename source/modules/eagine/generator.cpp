/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

export module eagine.shapes:generator;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.container;
import eagine.core.reflection;
import eagine.core.utility;
import eagine.core.runtime;
import eagine.core.math;
import eagine.core.valid_if;
import eagine.core.value_tree;
import eagine.core.main_ctx;
import eagine.core.resource;
export import :vertex_attributes;
export import :drawing;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
/// @brief Shape generator capability bit enumeration.
/// @ingroup shapes
/// @see generator_capabilities
export enum class generator_capability : std::uint16_t {
    /// @brief Indexed drawing of elements should be used if possible.
    indexed_drawing = 1U << 0U,
    /// @brief Line or triangle strips should be generated if possible.
    element_strips = 1U << 1U,
    /// @brief Line or triangle fanst should be generated if possible.
    element_fans = 1U << 2U,
    /// @brief Primitive restart functionality should be used if possible.
    primitive_restart = 1U << 3U,
    /// @brief Vertex attrib divisor functionality.
    attrib_divisors = 1U << 4U
};
//------------------------------------------------------------------------------
/// @brief Alias for generator_capability bitfield type.
/// @ingroup shapes
export using generator_capabilities = bitfield<generator_capability>;
//------------------------------------------------------------------------------
/// @brief Returns vertex_attrib_kinds value with all bits set.
/// @ingroup shapes
export [[nodiscard]] constexpr auto all_generator_capabilities() noexcept
  -> generator_capabilities {
    return generator_capabilities{(1U << 5U) - 1U};
}
//------------------------------------------------------------------------------
/// @brief Bitwise-or operator for generator_capability enumerators.
/// @ingroup shapes
export [[nodiscard]] constexpr auto operator|(
  const generator_capability a,
  const generator_capability b) noexcept -> generator_capabilities {
    return {a, b};
}
//------------------------------------------------------------------------------
} // namespace shapes
export template <typename Selector>
constexpr auto enumerator_mapping(
  const std::type_identity<shapes::generator_capability>,
  const Selector) noexcept {
    using shapes::generator_capability;
    return enumerator_map_type<generator_capability, 5>{
      {{"indexed_drawing", generator_capability::indexed_drawing},
       {"element_strips", generator_capability::element_strips},
       {"element_fans", generator_capability::element_fans},
       {"primitive_restart", generator_capability::primitive_restart},
       {"attrib_divisors", generator_capability::attrib_divisors}}};
}
namespace shapes {
//------------------------------------------------------------------------------
export struct shape_face_info {
    std::array<span_size_t, 3> indices{};
    bool cw_face_winding{};
};
//------------------------------------------------------------------------------
/// @brief Structure used to control generation of random shape attribute values.
/// @ingroup shapes
/// @see vertex_attrib_variant
/// @see generator
export struct random_attribute_values {
    /// @brief The number of random attribute values to be generated.
    span_size_t count{0};

    /// @brief Destination spans or float values for specified attribute variants.
    flat_map<vertex_attrib_variant, span<float>> float_values;
};
//------------------------------------------------------------------------------
/// @brief Alias for shape drawing variant index type.
/// @ingroup shapes
export using drawing_variant = span_size_t;
//------------------------------------------------------------------------------
/// @brief Interface for shape loaders or generators.
/// @ingroup shapes
export struct generator : abstract<generator> {

    /// @brief Returns the set of vertex attributes supported by this generator.
    /// @see attrib_count
    [[nodiscard]] virtual auto attrib_kinds() noexcept
      -> vertex_attrib_kinds = 0;

    /// @brief Tests if the specified attribute is supported by this generator.
    /// @see attrib_kinds
    /// @see attrib_count
    [[nodiscard]] auto has(const vertex_attrib_kind attrib) noexcept {
        return attrib_kinds().has(attrib);
    }

    /// @brief Returns the count of vertex attributes supported by this generator.
    /// @see attrib_kinds
    /// @see for_each_attrib
    [[nodiscard]] auto supported_attrib_count() noexcept -> span_size_t {
        return count_set_bits(attrib_kinds());
    }

    /// @brief Calls the specified function for each attribute kind.
    /// @see attrib_kinds
    /// @see supported_attrib_count
    void for_each_attrib(
      callable_ref<void(vertex_attrib_kinds, vertex_attrib_name_and_kind)> func) {
        for_each_bit(func, attrib_kinds());
    }

    /// @brief Enables or disables the specified generator capability.
    /// @see disable
    /// @see is_enabled
    virtual auto enable(
      const generator_capability cap,
      const bool value = true) noexcept -> bool = 0;

    /// @brief Disables the specified generator capability.
    /// @see enable
    /// @see is_enabled
    auto disable(const generator_capability cap) noexcept {
        return enable(cap, false);
    }

    /// @brief Indicates if the specified generator capability is enabled.
    /// @see enable
    /// @see disable
    [[nodiscard]] virtual auto is_enabled(
      const generator_capability cap) noexcept -> bool = 0;

    /// @brief Indicates if indexed drawing is enabled.
    [[nodiscard]] auto indexed_drawing(const drawing_variant var) noexcept
      -> bool {
        if(is_enabled(generator_capability::indexed_drawing)) {
            return index_count(var) > 0;
        }
        return false;
    }

    /// @brief Indicates if element strips are enabled.
    [[nodiscard]] auto strips_allowed() noexcept -> bool {
        return is_enabled(generator_capability::element_strips);
    }

    /// @brief Indicates if element fans are enabled.
    [[nodiscard]] auto fans_allowed() noexcept -> bool {
        return is_enabled(generator_capability::element_fans);
    }

    /// @brief Indicates if primitive restart is enabled.
    [[nodiscard]] auto primitive_restart() noexcept -> bool {
        return is_enabled(generator_capability::primitive_restart);
    }

    /// @brief Indicates if vertex attribute divisors is enabled.
    [[nodiscard]] auto attrib_divisors() noexcept -> bool {
        return is_enabled(generator_capability::attrib_divisors);
    }

    /// @brief Returns the instance count.
    [[nodiscard]] virtual auto instance_count() -> span_size_t = 0;

    /// @brief Returns the shape geometry vertex count.
    [[nodiscard]] virtual auto vertex_count() -> span_size_t = 0;

    /// @brief Returns the count of shape attribute variants.
    [[nodiscard]] virtual auto attribute_variants(const vertex_attrib_kind)
      -> span_size_t = 0;

    [[nodiscard]] auto has_variant(const vertex_attrib_variant vav) -> bool {
        assert(vav.has_valid_index());
        return vav.index() < attribute_variants(vav.attribute());
    }

    /// @brief Returns the name of the specified attribute variant.
    [[nodiscard]] virtual auto variant_name(const vertex_attrib_variant vav)
      -> string_view = 0;

    /// @brief Finds attribute variant by kind and name.
    [[nodiscard]] virtual auto find_variant(
      const vertex_attrib_kind attrib,
      const string_view name) -> vertex_attrib_variant = 0;

    /// @brief Finds attribute variant by name.
    [[nodiscard]] auto find_variant(const string_view name)
      -> vertex_attrib_variant;

    /// @brief Returns the number of values per vertex for the specified variant.
    [[nodiscard]] virtual auto values_per_vertex(const vertex_attrib_variant)
      -> span_size_t = 0;

    /// @brief Returns the total number of values for the specified attribute variant.
    [[nodiscard]] virtual auto value_count(const vertex_attrib_variant vav)
      -> span_size_t = 0;

    /// @brief Returns the attribute data type for the specified variant.
    [[nodiscard]] virtual auto attrib_type(const vertex_attrib_variant vav)
      -> attrib_data_type = 0;

    /// @brief Indicates if the specified variant attribute values are integral.
    [[nodiscard]] virtual auto is_attrib_integral(
      const vertex_attrib_variant vav) -> bool = 0;

    /// @brief Indicates if the specified variant attribute values should be normalized.
    [[nodiscard]] virtual auto is_attrib_normalized(
      const vertex_attrib_variant vav) -> bool = 0;

    /// @brief Returns the vertex attribute divisor value.
    [[nodiscard]] virtual auto attrib_divisor(const vertex_attrib_variant vav)
      -> std::uint32_t = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as bytes.
    virtual void attrib_values(const vertex_attrib_variant, span<byte> dest) = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as integers.
    virtual void attrib_values(
      const vertex_attrib_variant,
      span<std::int16_t>) = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as integers.
    virtual void attrib_values(
      const vertex_attrib_variant,
      span<std::int32_t>) = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as integers.
    virtual void attrib_values(
      const vertex_attrib_variant,
      span<std::uint16_t>) = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as integers.
    virtual void attrib_values(
      const vertex_attrib_variant,
      span<std::uint32_t>) = 0;

    /// @brief Fetches the vertex attribute data for the specified variant as floats.
    virtual void attrib_values(
      const vertex_attrib_variant,
      span<float> dest) = 0;

    /// @brief Returns the count of possible shape draw variants.
    [[nodiscard]] virtual auto draw_variant_count() -> span_size_t = 0;

    /// @brief Returns the identifier of the drawing variant at the specified index.
    [[nodiscard]] auto draw_variant(const span_size_t index)
      -> drawing_variant {
        return index;
    }

    /// @brief Returns the index data type for the specified draw variant.
    [[nodiscard]] virtual auto index_type(const drawing_variant)
      -> index_data_type = 0;

    /// @brief Returns the index data type for the default draw variant.
    [[nodiscard]] auto index_type() -> index_data_type {
        return index_type(0);
    }

    /// @brief Returns the index count for the specified drawing variant.
    [[nodiscard]] virtual auto index_count(const drawing_variant)
      -> span_size_t = 0;

    /// @brief Returns the index count for the default drawing variant.
    [[nodiscard]] auto index_count() {
        return index_count(0);
    }

    /// @brief Fetches the index data for the specified drawing variant.
    virtual void indices(const drawing_variant, span<std::uint8_t> dest) = 0;

    /// @brief Fetches the index data for the default drawing variant.
    void indices(span<std::uint8_t> dest) {
        indices(0, dest);
    }

    /// @brief Fetches the index data for the specified drawing variant.
    virtual void indices(const drawing_variant, span<std::uint16_t> dest) = 0;

    /// @brief Fetches the index data for the default drawing variant.
    void indices(span<std::uint16_t> dest) {
        indices(0, dest);
    }

    /// @brief Fetches the index data for the specified drawing variant.
    virtual void indices(const drawing_variant, span<std::uint32_t> dest) = 0;

    /// @brief Fetches the index data for the default drawing variant.
    void indices(span<std::uint32_t> dest) {
        indices(0, dest);
    }

    /// @brief Returns the number of drawing instructions for the specified variant.
    [[nodiscard]] virtual auto operation_count(const drawing_variant)
      -> span_size_t = 0;

    /// @brief Returns the number of drawing instructions for the default variant.
    [[nodiscard]] auto operation_count() {
        return operation_count(0);
    }

    /// @brief Fetches the drawing operations for the specified drawing variant.
    virtual void instructions(
      const drawing_variant,
      span<draw_operation> dest) = 0;

    /// @brief Fetches the drawing operations for the default drawing variant.
    void instructions(span<draw_operation> dest) {
        return instructions(0, dest);
    }

    /// @brief Returns the bounding sphere for the generated shape.
    virtual auto bounding_sphere() -> math::sphere<float, true> = 0;

    /// @brief Calls a callback for each triangle in specified drawing variant.
    virtual void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback) = 0;

    /// @brief Calls a callback for each triangle in the default drawing variant.
    void for_each_triangle(
      const callable_ref<void(const shape_face_info&)> callback) {
        for_each_triangle(*this, 0, callback);
    }

    /// @brief Checks if the structure for random values is consistent.
    /// @see random_attribute_values
    [[nodiscard]] auto are_consistent(
      const random_attribute_values& values) noexcept -> bool {
        for(const auto& [vav, dest] : values.float_values) {
            if(dest.size() < values.count * values_per_vertex(vav)) {
                return false;
            }
        }
        return true;
    }

    /// @brief Generates attribute values at random surface points.
    /// @see are_consistent
    virtual void random_surface_values(const random_attribute_values&) = 0;

    /// @brief Calculates the intersections of the shape geometry with a ray.
    virtual void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) = 0;

    /// @brief Calculates the intersections of the shape geometry with a ray.
    void ray_intersections(
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) {
        return ray_intersections(*this, 0, rays, intersections);
    }

    /// @brief Returns the parameter for the nearest intersection with a ray.
    [[nodiscard]] auto ray_intersection(
      const drawing_variant var,
      const math::line<float, true>& ray) -> optionally_valid<float> {
        optionally_valid<float> result{};
        ray_intersections(*this, var, view_one(ray), cover_one(result));
        return result;
    }

    /// @brief Returns the parameter for the nearest intersection with a ray.
    [[nodiscard]] auto ray_intersection(const math::line<float, true>& ray)
      -> optionally_valid<float> {
        optionally_valid<float> result{};
        ray_intersections(*this, 0, view_one(ray), cover_one(result));
        return result;
    }
};
//------------------------------------------------------------------------------
/// @brief Common base implementation of the shape generator interface.
/// @ingroup shapes
class generator_base : public generator {
public:
    auto attrib_kinds() noexcept -> vertex_attrib_kinds final;
    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final;

    auto is_enabled(const generator_capability cap) noexcept -> bool final;

    auto instance_count() -> span_size_t override;

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override;

    auto variant_name(const vertex_attrib_variant) -> string_view override;

    auto find_variant(const vertex_attrib_kind attrib, const string_view name)
      -> vertex_attrib_variant override;

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override;

    auto value_count(const vertex_attrib_variant vav) -> span_size_t override;

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override;

    auto is_attrib_integral(const vertex_attrib_variant vav) -> bool override;

    auto is_attrib_normalized(const vertex_attrib_variant) -> bool override;

    auto attrib_divisor(const vertex_attrib_variant) -> std::uint32_t override;

    void attrib_values(const vertex_attrib_variant, span<byte>) override;

    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) override;

    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;

    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>)
      override;

    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>)
      override;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto draw_variant_count() -> span_size_t override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

    void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback) override;

    void random_surface_values(const random_attribute_values&) override;

    void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<optionally_valid<float>> intersections) override;

protected:
    generator_base(
      const vertex_attrib_kinds attr_kinds,
      const generator_capabilities supported_caps) noexcept;

private:
    vertex_attrib_kinds _attr_kinds;
    const generator_capabilities _supported_caps;
    generator_capabilities _enabled_caps{_supported_caps};
};
//------------------------------------------------------------------------------
export auto operator+(
  shared_holder<generator>&& l,
  shared_holder<generator>&& r) noexcept
  -> std::array<shared_holder<generator>, 2> {
    return {{std::move(l), std::move(r)}};
}
//------------------------------------------------------------------------------
export template <std::size_t N, std::size_t... I>
auto _add_to_array(
  std::array<shared_holder<generator>, N>&& l,
  shared_holder<generator>&& r,
  const std::index_sequence<I...>) noexcept
  -> std::array<shared_holder<generator>, N + 1> {
    return {{std::move(l[I])..., std::move(r)}};
}
//------------------------------------------------------------------------------
export template <std::size_t N>
auto operator+(
  std::array<shared_holder<generator>, N>&& l,
  shared_holder<generator>&& r) noexcept
  -> std::array<shared_holder<generator>, N + 1> {
    return _add_to_array(
      std::move(l), std::move(r), std::make_index_sequence<N>());
}
//------------------------------------------------------------------------------
// generator_base
//------------------------------------------------------------------------------
inline auto generator_base::attrib_kinds() noexcept -> vertex_attrib_kinds {
    return _attr_kinds;
}
//------------------------------------------------------------------------------
inline auto generator_base::enable(
  const generator_capability cap,
  const bool value) noexcept -> bool {
    bool result{true};
    if(value) {
        if(_supported_caps.has(cap)) {
            _enabled_caps.set(cap);
        } else {
            result = false;
        }
    } else {
        _enabled_caps.clear(cap);
    }
    return result;
}
//------------------------------------------------------------------------------
inline auto generator_base::is_enabled(const generator_capability cap) noexcept
  -> bool {
    return _enabled_caps.has(cap);
}
//------------------------------------------------------------------------------
inline auto generator_base::instance_count() -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
inline auto generator_base::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    return has(attrib) ? 1 : 0;
}
//------------------------------------------------------------------------------
inline auto generator_base::variant_name(const vertex_attrib_variant)
  -> string_view {
    return {};
}
//------------------------------------------------------------------------------
inline auto generator_base::find_variant(
  const vertex_attrib_kind attrib,
  const string_view name) -> vertex_attrib_variant {
    const span_size_t n = attribute_variants(attrib);
    span_size_t index{-1};
    for(const auto i : integer_range(n)) {
        if(are_equal(name, variant_name({attrib, i}))) {
            index = i;
            break;
        }
    }
    return {attrib, index};
}
//------------------------------------------------------------------------------
inline auto generator_base::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    return has_variant(vav) ? attrib_values_per_vertex(vav) : 0U;
}
//------------------------------------------------------------------------------
inline auto generator_base::value_count(const vertex_attrib_variant vav)
  -> span_size_t {
    return vertex_count() * values_per_vertex(vav);
}
//------------------------------------------------------------------------------
inline auto generator_base::attrib_type(const vertex_attrib_variant)
  -> attrib_data_type {
    return attrib_data_type::float_;
}
//------------------------------------------------------------------------------
inline auto generator_base::is_attrib_integral(const vertex_attrib_variant vav)
  -> bool {
    switch(attrib_type(vav)) {
        case attrib_data_type::ubyte:
        case attrib_data_type::int_16:
        case attrib_data_type::int_32:
        case attrib_data_type::uint_16:
        case attrib_data_type::uint_32:
            return true;
        default:
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
inline auto generator_base::is_attrib_normalized(const vertex_attrib_variant)
  -> bool {
    return false;
}
//------------------------------------------------------------------------------
inline auto generator_base::attrib_divisor(const vertex_attrib_variant)
  -> std::uint32_t {
    return 0U;
}
//------------------------------------------------------------------------------
inline void generator_base::attrib_values(
  const vertex_attrib_variant,
  span<byte>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline void generator_base::attrib_values(
  const vertex_attrib_variant,
  span<std::int16_t>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline void generator_base::attrib_values(
  const vertex_attrib_variant,
  span<std::int32_t>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline void generator_base::attrib_values(
  const vertex_attrib_variant,
  span<std::uint16_t>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline void generator_base::attrib_values(
  const vertex_attrib_variant,
  span<std::uint32_t>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline auto generator_base::draw_variant_count() -> span_size_t {
    return 1;
}
//------------------------------------------------------------------------------
inline auto generator_base::index_type(const drawing_variant)
  -> index_data_type {
    return index_data_type::none;
}
//------------------------------------------------------------------------------
inline auto generator_base::index_count(const drawing_variant) -> span_size_t {
    return 0;
}
//------------------------------------------------------------------------------
inline void generator_base::indices(const drawing_variant, span<std::uint8_t>) {
    unreachable();
}
//------------------------------------------------------------------------------
inline void generator_base::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    if(index_type(var) == index_data_type::unsigned_8) {
        std::vector<std::uint8_t> tmp;
        tmp.resize(integer(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    }
}
//------------------------------------------------------------------------------
inline void generator_base::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    const auto ity = index_type(var);
    if(ity == index_data_type::unsigned_8) {
        std::vector<std::uint8_t> tmp;
        tmp.resize(integer(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    } else if(ity == index_data_type::unsigned_16) {
        std::vector<std::uint16_t> tmp;
        tmp.resize(integer(index_count(var)));
        indices(var, cover(tmp));
        copy(view(tmp), dest);
    }
}
//------------------------------------------------------------------------------
inline void generator_base::random_surface_values(
  [[maybe_unused]] const random_attribute_values& values) {
    assert(are_consistent(values));
}
//------------------------------------------------------------------------------
// screen
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_screen_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_screen(const vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator>;

/// @brief Tries to construct instances of unit_screen_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_screen_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// plane
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_plane_gen.
/// @ingroup shapes
/// @see unit_plane_from
/// @see from_value_tree
/// @see unit_screen
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_plane(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int>& width,
  const valid_if_positive<int>& height) -> shared_holder<generator>;

export [[nodiscard]] auto unit_plane(const vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator> {
    return unit_plane(attr_kinds, 4, 4);
}

/// @brief Tries to construct instances of unit_plane_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_plane_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// cube
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_cube_gen.
/// @ingroup shapes
/// @see unit_cube_from
/// @see from_value_tree
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto unit_cube(const vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator>;

/// @brief Tries to construct instances of unit_cube_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_cube_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// round_cube
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_round_cube_gen.
/// @ingroup shapes
/// @see unit_round_cube_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto unit_round_cube(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_positive<int> divisions) -> shared_holder<generator>;
//------------------------------------------------------------------------------
export [[nodiscard]] auto unit_round_cube(const vertex_attrib_kinds attr_kinds) {
    return unit_round_cube(attr_kinds, 8);
}

/// @brief Tries to construct instances of unit_round_cube_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_round_cube_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// skybox
//------------------------------------------------------------------------------
/// @brief Constructs instances of skybox_gen
/// @ingroup shapes
/// @see skybox_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_sphere
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto skybox(const vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator>;

/// @brief Tries to construct instances of skybox_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto skybox_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// icosahedron
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_icosahedron_gen.
/// @ingroup shapes
/// @see unit_icosahedron_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_torus
/// @see unit_screen
/// @see unit_twisted_torus
export [[nodiscard]] auto unit_icosahedron(vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator>;

/// @brief Tries to construct instances of unit_icosahedron_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_icosahedron_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// tetrahedrons
//------------------------------------------------------------------------------
/// @brief Constructs instances of marching_tetrahedrons_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_icosahedron
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_torus
/// @see unit_screen
/// @see unit_twisted_torus
export [[nodiscard]] auto marching_tetrahedrons(vertex_attrib_kinds attr_kinds)
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// sphere
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see unit_sphere_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_screen
export [[nodiscard]] auto unit_sphere(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections) -> shared_holder<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_sphere_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_twisted_torus
/// @see unit_icosahedron
/// @see unit_screen
export [[nodiscard]] auto unit_sphere(const vertex_attrib_kinds attr_kinds) {
    return unit_sphere(attr_kinds, 18, 36);
}

/// @brief Tries to construct instances of unit_sphere_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_sphere_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// torus
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_torus_gen.
/// @ingroup shapes
export [[nodiscard]] auto unit_torus(
  const vertex_attrib_kinds attr_kinds,
  const valid_if_greater_than<int, 4>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) -> shared_holder<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_torus_gen.
/// @ingroup shapes
/// @see unit_torus_from
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_twisted_torus
/// @see unit_icosahedron
/// @see unit_screen
export [[nodiscard]] auto unit_torus(const vertex_attrib_kinds attr_kinds) {
    return unit_torus(attr_kinds, 18, 36, 0.5F);
}

/// @brief Tries to construct instances of unit_torus_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_torus_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// twisted_torus
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_icosahedron
/// @see unit_torus
/// @see unit_screen
export [[nodiscard]] auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds,
  const int twist,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) -> shared_holder<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of unit_twisted_torus_gen.
/// @ingroup shapes
/// @see from_value_tree
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_torus
/// @see unit_icosahedron
/// @see unit_screen
export [[nodiscard]] auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds) {
    return unit_twisted_torus(attr_kinds, 12, 38, 12, 0.5F);
}

/// @brief Tries to construct instances of unit_twisted_torus_gen from an URL.
/// @ingroup shapes
export [[nodiscard]] auto unit_twisted_torus_from(
  const vertex_attrib_kinds,
  const url&,
  main_ctx&) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// combined
//------------------------------------------------------------------------------
export [[nodiscard]] auto combine(shared_holder<generator>&& gen)
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
export template <std::size_t N>
[[nodiscard]] auto combine(std::array<shared_holder<generator>, N>&& gens)
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// cached
//------------------------------------------------------------------------------
/// @brief Constructs instances of cached_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto cache(
  shared_holder<generator> gen,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// array
//------------------------------------------------------------------------------
export [[nodiscard]] auto array(
  shared_holder<generator> gen,
  const std::array<float, 3> d,
  const span_size_t count) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
export [[nodiscard]] auto ortho_array_xyz(
  shared_holder<generator> gen,
  const std::array<float, 3> d,
  const std::array<span_size_t, 3> n) noexcept {
    const float z = 0.0F;
    return array(
      array(array(std::move(gen), {d[0], z, z}, n[0]), {z, d[1], z}, n[1]),
      {z, z, d[2]},
      n[2]);
}
//------------------------------------------------------------------------------
// centered
//------------------------------------------------------------------------------
/// @brief Constructs instances of centered_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto center(shared_holder<generator> gen) noexcept
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// translated
//------------------------------------------------------------------------------
/// @brief Constructs instances of translated_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto translate(
  shared_holder<generator> gen,
  std::array<float, 3> d) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// scaled
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto scale(
  shared_holder<generator> gen,
  const std::array<float, 3> s) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// scaled_wrap_coords
//------------------------------------------------------------------------------
/// @brief Constructs instances of scaled_wrap_coords_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto scale_wrap_coords(
  shared_holder<generator> gen,
  float x,
  float y,
  float z) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// reboxed
//------------------------------------------------------------------------------
/// @brief Constructs instances of reboxed_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto rebox(shared_holder<generator> gen) noexcept
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// occluded
//------------------------------------------------------------------------------
/// @brief Constructs instances of occluded_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto occlude(
  shared_holder<generator> gen,
  const span_size_t samples,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// value_tree
//------------------------------------------------------------------------------
/// @brief Constructs instances of value_tree_loader.
/// @ingroup shapes
/// @see unit_cube
/// @see unit_round_cube
/// @see unit_sphere
/// @see unit_icosahedron
/// @see unit_screen
/// @see unit_torus
/// @see unit_twisted_torus
export [[nodiscard]] auto from_value_tree(
  valtree::compound source,
  main_ctx_parent parent) -> shared_holder<generator>;
//------------------------------------------------------------------------------
// from_json
//------------------------------------------------------------------------------
export [[nodiscard]] auto from_json_stream(std::istream&, main_ctx&) noexcept
  -> shared_holder<generator>;
export [[nodiscard]] auto from_json_resource(
  const embedded_resource&,
  main_ctx&) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
export [[nodiscard]] auto model_cube(main_ctx&) noexcept
  -> shared_holder<generator>;
export [[nodiscard]] auto model_cube_from(const url&, main_ctx&) noexcept
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// surface_points
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto surface_points(
  shared_holder<generator> gen,
  const span_size_t point_count,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instance of surface_points_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto surface_points(
  shared_holder<generator> gen,
  const span_size_t point_count,
  const vertex_attrib_variant weight_variant,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
// add_triangle_adjacency
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto add_triangle_adjacency(
  shared_holder<generator> gen,
  const drawing_variant var,
  main_ctx_parent parent) noexcept -> shared_holder<generator>;
//------------------------------------------------------------------------------
/// @brief Constructs instances of triangle_adjacency_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto add_triangle_adjacency(
  shared_holder<generator> gen,
  main_ctx_parent parent) noexcept {
    return add_triangle_adjacency(std::move(gen), 0, parent);
}
//------------------------------------------------------------------------------
// to_patches
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_patches_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto to_patches(shared_holder<generator> gen) noexcept
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
// to_quads
//------------------------------------------------------------------------------
/// @brief Constructs instances of to_quads_gen modifier.
/// @ingroup shapes
export [[nodiscard]] auto to_quads(shared_holder<generator> gen) noexcept
  -> shared_holder<generator>;
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine

