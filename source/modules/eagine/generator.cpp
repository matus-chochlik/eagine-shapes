/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

export module eagine.shapes:generator;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.container;
import eagine.core.reflection;
import eagine.core.utility;
import eagine.core.math;
import :generator_capabilities;
import :vertex_attrib;
import :drawing;
import <array>;
import <memory>;
import <optional>;

namespace eagine::shapes {
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
export struct generator : interface<generator> {

    /// @brief Returns the set of vertex attributes supported by this generator.
    /// @see attrib_count
    virtual auto attrib_kinds() noexcept -> vertex_attrib_kinds = 0;

    /// @brief Tests if the specified attribute is supported by this generator.
    /// @see attrib_kinds
    /// @see attrib_count
    auto has(const vertex_attrib_kind attrib) noexcept {
        return attrib_kinds().has(attrib);
    }

    /// @brief Returns the count of vertex attributes supported by this generator.
    /// @see attrib_kinds
    /// @see for_each_attrib
    auto supported_attrib_count() noexcept -> span_size_t {
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
    virtual auto is_enabled(const generator_capability cap) noexcept
      -> bool = 0;

    /// @brief Indicates if indexed drawing is enabled.
    auto indexed_drawing(const drawing_variant var) noexcept -> bool {
        if(is_enabled(generator_capability::indexed_drawing)) {
            return index_count(var) > 0;
        }
        return false;
    }

    /// @brief Indicates if element strips are enabled.
    auto strips_allowed() noexcept -> bool {
        return is_enabled(generator_capability::element_strips);
    }

    /// @brief Indicates if element fans are enabled.
    auto fans_allowed() noexcept -> bool {
        return is_enabled(generator_capability::element_fans);
    }

    /// @brief Indicates if primitive restart is enabled.
    auto primitive_restart() noexcept -> bool {
        return is_enabled(generator_capability::primitive_restart);
    }

    /// @brief Indicates if vertex attribute divisors is enabled.
    auto attrib_divisors() noexcept -> bool {
        return is_enabled(generator_capability::attrib_divisors);
    }

    /// @brief Returns the instance count.
    virtual auto instance_count() -> span_size_t = 0;

    /// @brief Returns the shape geometry vertex count.
    virtual auto vertex_count() -> span_size_t = 0;

    /// @brief Returns the count of shape attribute variants.
    virtual auto attribute_variants(const vertex_attrib_kind)
      -> span_size_t = 0;

    auto has_variant(const vertex_attrib_variant vav) -> bool {
        assert(vav.has_valid_index());
        return vav.index() < attribute_variants(vav.attribute());
    }

    /// @brief Returns the name of the specified attribute variant.
    virtual auto variant_name(const vertex_attrib_variant vav)
      -> string_view = 0;

    /// @brief Finds attribute variant by kind and name.
    auto find_variant(const vertex_attrib_kind attrib, const string_view name)
      -> vertex_attrib_variant;

    /// @brief Finds attribute variant by name.
    auto find_variant(const string_view name) -> vertex_attrib_variant;

    /// @brief Returns the number of values per vertex for the specified variant.
    virtual auto values_per_vertex(const vertex_attrib_variant)
      -> span_size_t = 0;

    /// @brief Returns the total number of values for the specified attribute variant.
    virtual auto value_count(const vertex_attrib_variant vav) -> span_size_t {
        return vertex_count() * values_per_vertex(vav);
    }

    /// @brief Returns the attribute data type for the specified variant.
    virtual auto attrib_type(const vertex_attrib_variant vav)
      -> attrib_data_type = 0;

    /// @brief Indicates if the specified variant attribute values are integral.
    virtual auto is_attrib_integral(const vertex_attrib_variant vav)
      -> bool = 0;

    /// @brief Indicates if the specified variant attribute values should be normalized.
    virtual auto is_attrib_normalized(const vertex_attrib_variant vav)
      -> bool = 0;

    /// @brief Returns the vertex attribute divisor value.
    virtual auto attrib_divisor(const vertex_attrib_variant vav)
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
    virtual auto draw_variant_count() -> span_size_t = 0;

    /// @brief Returns the identifier of the drawing variant at the specified index.
    auto draw_variant(const span_size_t index) -> drawing_variant {
        return index;
    }

    /// @brief Returns the index data type for the specified draw variant.
    virtual auto index_type(const drawing_variant) -> index_data_type = 0;

    /// @brief Returns the index data type for the default draw variant.
    auto index_type() -> index_data_type {
        return index_type(0);
    }

    /// @brief Returns the index count for the specified drawing variant.
    virtual auto index_count(const drawing_variant) -> span_size_t = 0;

    /// @brief Returns the index count for the default drawing variant.
    auto index_count() {
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
    virtual auto operation_count(const drawing_variant) -> span_size_t = 0;

    /// @brief Returns the number of drawing instructions for the default variant.
    auto operation_count() {
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
    virtual auto bounding_sphere() -> math::sphere<float, true>;

    /// @brief Calls a callback for each triangle in specified drawing variant.
    virtual void for_each_triangle(
      generator& gen,
      const drawing_variant var,
      const callable_ref<void(const shape_face_info&)> callback);

    /// @brief Calls a callback for each triangle in the default drawing variant.
    void for_each_triangle(
      const callable_ref<void(const shape_face_info&)> callback) {
        for_each_triangle(*this, 0, callback);
    }

    /// @brief Checks if the structure for random values is consistent.
    /// @see random_attribute_values
    auto are_consistent(const random_attribute_values& values) noexcept
      -> bool {
        for(const auto& [vav, dest] : values.float_values) {
            if(dest.size() < values.count * values_per_vertex(vav)) {
                return false;
            }
        }
        return true;
    }

    /// @brief Generates attribute values at random surface points.
    /// @see are_consistent
    virtual void random_surface_values(const random_attribute_values&);

    /// @brief Calculates the intersections of the shape geometry with a ray.
    virtual void ray_intersections(
      generator&,
      const drawing_variant,
      const span<const math::line<float, true>> rays,
      span<std::optional<float>> intersections);

    /// @brief Calculates the intersections of the shape geometry with a ray.
    void ray_intersections(
      const span<const math::line<float, true>> rays,
      span<std::optional<float>> intersections) {
        return ray_intersections(*this, 0, rays, intersections);
    }

    /// @brief Returns the parameter for the nearest intersection with a ray.
    auto ray_intersection(
      const drawing_variant var,
      const math::line<float, true>& ray) -> std::optional<float> {
        std::optional<float> result{};
        ray_intersections(*this, var, view_one(ray), cover_one(result));
        return result;
    }

    /// @brief Returns the parameter for the nearest intersection with a ray.
    auto ray_intersection(const math::line<float, true>& ray)
      -> std::optional<float> {
        std::optional<float> result{};
        ray_intersections(*this, 0, view_one(ray), cover_one(result));
        return result;
    }
};
//------------------------------------------------------------------------------
/// @brief Common base implementation of the shape generator interface.
/// @ingroup shapes
export class generator_base : public generator {
public:
    auto attrib_kinds() noexcept -> vertex_attrib_kinds final {
        return _attr_kinds;
    }

    auto enable(const generator_capability cap, const bool value) noexcept
      -> bool final {
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

    auto is_enabled(const generator_capability cap) noexcept -> bool final {
        return _enabled_caps.has(cap);
    }

    auto instance_count() -> span_size_t override {
        return 1;
    }

    auto attribute_variants(const vertex_attrib_kind attrib)
      -> span_size_t override {
        return has(attrib) ? 1 : 0;
    }

    auto variant_name(const vertex_attrib_variant) -> string_view override {
        return {};
    }

    auto values_per_vertex(const vertex_attrib_variant vav)
      -> span_size_t override {
        return has_variant(vav) ? attrib_values_per_vertex(vav) : 0U;
    }

    auto attrib_type(const vertex_attrib_variant) -> attrib_data_type override {
        return attrib_data_type::float_;
    }

    auto is_attrib_integral(const vertex_attrib_variant vav) -> bool override {
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

    auto is_attrib_normalized(const vertex_attrib_variant) -> bool override {
        return false;
    }

    auto attrib_divisor(const vertex_attrib_variant) -> std::uint32_t override {
        return 0U;
    }

    void attrib_values(const vertex_attrib_variant, span<byte>) override {
        unreachable();
    }

    void attrib_values(const vertex_attrib_variant, span<std::int16_t>)
      override {
        unreachable();
    }

    void attrib_values(const vertex_attrib_variant, span<std::int32_t>)
      override {
        unreachable();
    }

    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>)
      override {
        unreachable();
    }

    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>)
      override {
        unreachable();
    }

    void attrib_values(const vertex_attrib_variant, span<float>) override {
        unreachable();
    }

    auto draw_variant_count() -> span_size_t override {
        return 1;
    }

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;

    void indices(const drawing_variant, span<std::uint32_t> dest) override;

protected:
    generator_base(
      const vertex_attrib_kinds attr_kinds,
      const generator_capabilities supported_caps) noexcept
      : _attr_kinds{attr_kinds}
      , _supported_caps{supported_caps} {}

private:
    vertex_attrib_kinds _attr_kinds;
    const generator_capabilities _supported_caps;
    generator_capabilities _enabled_caps{_supported_caps};
};
//------------------------------------------------------------------------------
/// @brief Base class for shape generators re-calculating the center.
/// @ingroup shapes
export class centered_unit_shape_generator_base : public generator_base {
public:
    using generator_base::attrib_values;
    void attrib_values(const vertex_attrib_variant vav, span<float> dest)
      override;

protected:
    centered_unit_shape_generator_base(
      const vertex_attrib_kinds attr_kinds,
      const generator_capabilities supported_caps) noexcept
      : generator_base(attr_kinds, supported_caps) {}
};
//------------------------------------------------------------------------------
export auto operator+(
  std::unique_ptr<generator>&& l,
  std::unique_ptr<generator>&& r) noexcept
  -> std::array<std::unique_ptr<generator>, 2> {
    return {{std::move(l), std::move(r)}};
}
//------------------------------------------------------------------------------
export template <std::size_t N, std::size_t... I>
auto _add_to_array(
  std::array<std::unique_ptr<generator>, N>&& l,
  std::unique_ptr<generator>&& r,
  const std::index_sequence<I...>) noexcept
  -> std::array<std::unique_ptr<generator>, N + 1> {
    return {{std::move(l[I])..., std::move(r)}};
}
//------------------------------------------------------------------------------
export template <std::size_t N>
auto operator+(
  std::array<std::unique_ptr<generator>, N>&& l,
  std::unique_ptr<generator>&& r) noexcept
  -> std::array<std::unique_ptr<generator>, N + 1> {
    return _add_to_array(
      std::move(l), std::move(r), std::make_index_sequence<N>());
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

