/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

export module eagine.shapes:topology;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.container;
import eagine.core.identifier;
import eagine.core.math;
import eagine.core.main_ctx;
import :generator;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export struct topology_data;
export class mesh_triangle;

/// @brief Class storing information about an edge between two mesh faces.
/// @ingroup  shapes
/// @see mesh_triangle
/// @see topology
export class mesh_edge {
public:
    mesh_edge(
      mesh_triangle& a,
      const std::uint8_t aeb,
      const std::uint8_t aee,
      mesh_triangle& b,
      const std::uint8_t beb,
      const std::uint8_t bee) noexcept
      : _triangles{{&a, &b}}
      , _edge_indices{{aeb, aee, beb, bee}} {}

    /// @brief Returns one of the two adjacent triangle faces.
    /// @pre i >= 0 and i < 2
    auto triangle(const span_size_t i) const noexcept -> const mesh_triangle& {
        assert(i >= 0 and i < 2);
        return *_triangles[integer(i)];
    }

    /// @brief Returns a pair of vertex indices (0,1 or 2) defining the i-th edge.
    /// @pre i >= 0 and i < 2
    auto edge_vertices(const span_size_t i) const noexcept
      -> std::tuple<unsigned, unsigned> {
        assert(i >= 0 and i < 2);
        return {
          _edge_indices[integer(2 * i)], _edge_indices[integer(2 * i + 1)]};
    }

private:
    std::array<mesh_triangle*, 2> _triangles;
    std::array<std::uint8_t, 4> _edge_indices;
};
//------------------------------------------------------------------------------
/// @brief Class storing information about a mesh triangular face.
/// @ingroup  shapes
/// @see mesh_edge
/// @see topology
export class mesh_triangle {
public:
    mesh_triangle(
      const std::size_t tri_idx,
      const unsigned a,
      const unsigned b,
      const unsigned c) noexcept
      : _tri_idx{tri_idx}
      , _indices{{a, b, c}} {}

    /// @brief Returns the index of this triangle within the mesh.
    auto index() const noexcept -> span_size_t {
        return span_size(_tri_idx);
    }

    auto setup_adjacent(mesh_triangle& that, const topology_data& d) noexcept
      -> std::tuple<bool, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>;

    /// @brief Returns the v-th vertex index.
    /// @pre v >= 0 and v < 3
    auto vertex_index(const span_size_t v) const noexcept {
        assert(v >= 0 and v < 3);
        return _indices[integer(v)];
    }

    /// @brief Returns the triangle adjacent through the v-th edge.
    /// @pre v >= 0 and v < 3
    /// @see opposite_vertex
    auto adjacent_triangle(const span_size_t v) const noexcept
      -> const mesh_triangle* {
        assert(v >= 0 and v < 3);
        return _adjacent[integer(v)];
    }

    /// @brief Returns opposite vertex index (0,1 or 2) in the v-th adjacent triangle.
    /// @pre v >= 0 and v < 3
    /// @see adjacent_triangle
    /// @see opposite_index
    auto opposite_vertex(const span_size_t v) const noexcept -> unsigned {
        assert(v >= 0 and v < 3);
        return _opposite[integer(v)];
    }

    /// @brief Returns the in-mesh index of the v-th adjacent vertex.
    /// @pre v >= 0 and v < 3
    /// @see opposite_vertex
    /// @see adjacent_triangle
    auto opposite_index(const span_size_t v) const noexcept -> unsigned {
        if(auto tri{adjacent_triangle(v)}) {
            return tri->vertex_index(opposite_vertex(v));
        }
        return _indices[(v + 2) % 3];
    }

    auto set_area(float a) noexcept -> auto& {
        _area = a;
        return *this;
    }

    /// @brief Returns the area of the triangle.
    auto area() const noexcept -> float {
        return _area;
    }

    auto set_weight(float w) noexcept -> auto& {
        _weight = w;
        return *this;
    }

    /// @brief Returns the weight of the triangle.
    auto weight() const noexcept -> float {
        return _weight;
    }

private:
    auto curri(const std::size_t i) const noexcept -> unsigned {
        return _indices[i];
    }

    static auto prevv(const std::size_t i) noexcept -> unsigned {
        return (i + 2) % 3;
    }

    auto previ(const std::size_t i) const noexcept -> unsigned {
        return _indices[prevv(i)];
    }

    static auto nextv(const std::size_t i) noexcept -> unsigned {
        return (i + 1) % 3;
    }

    auto nexti(const std::size_t i) const noexcept -> unsigned {
        return _indices[nextv(i)];
    }

    static auto narrow(const std::size_t i) noexcept -> std::uint8_t {
        return limit_cast<std::uint8_t>(i);
    }

    std::size_t _tri_idx{~0U};
    std::array<unsigned, 3> _indices{};
    float _area{1.F};
    float _weight{1.F};
    std::array<mesh_triangle*, 3> _adjacent{{nullptr, nullptr, nullptr}};
    std::array<std::uint8_t, 3> _opposite{{0, 0, 0}};
};
//------------------------------------------------------------------------------
/// @brief Enumeration of shape topology features that can be analysed.
/// @ingroup shapes
/// @see topology
export enum class topology_feature_bit : unsigned {
    /// @brief Shape triangle adjacency
    triangle_adjacency = 1U << 0U,
    /// @brief Shape triangle area.
    triangle_area = 1U << 1U,
    /// @brief Shape triangle weight (from vertex weight).
    triangle_weight = 1U << 2U,
    /// @brief Shape triangle area.
    edge_length = 1U << 3U
};
//------------------------------------------------------------------------------
/// @brief Shape topology features bitfield.
/// @ingroup shapes
/// @see topology
export using topology_feature_bits = bitfield<topology_feature_bit>;
/// @brief Returns vertex_attrib_kinds value with all bits set.
/// @ingroup shapes
export constexpr auto all_topology_features() noexcept
  -> topology_feature_bits {
    return topology_feature_bits{(1U << 4U) - 1U};
}
//------------------------------------------------------------------------------
/// @brief Bitwise-or operator for vertex_attrib_kind bits.
/// @ingroup shapes
export constexpr auto operator|(
  const topology_feature_bit a,
  const topology_feature_bit b) noexcept -> topology_feature_bits {
    return {a, b};
}
//------------------------------------------------------------------------------
export struct topology_options {
    span_size_t draw_variant_index{0};
    vertex_attrib_variant position_variant{vertex_attrib_kind::position, 0};
    vertex_attrib_variant weight_variant{vertex_attrib_kind::weight, 0};
    topology_feature_bits features{};

    constexpr topology_options() noexcept = default;
};
//------------------------------------------------------------------------------
/// @brief Class holding information about the topology of a generated shape.
/// @ingroup shapes
/// @see mesh_edge
/// @see mesh_triangle
export class topology : public main_ctx_object {
public:
    /// @brief Construction from a shape generator and options.
    topology(
      shared_holder<generator> gen,
      const topology_options& opts,
      main_ctx_parent parent);

    /// @brief Returns the number of triangles in the mesh.
    auto triangle_count() const noexcept -> span_size_t {
        return span_size(_triangles.size());
    }

    /// @brief Returns the i-th triangle in the mesh.
    auto triangle(const span_size_t i) const noexcept -> const mesh_triangle& {
        return _triangles[integer(i)];
    }

    auto print_dot(std::ostream& out) const -> std::ostream&;

private:
    template <std::integral I>
    static auto to_index(const I i) noexcept -> unsigned {
        return limit_cast<unsigned>(i);
    }

    void _scan_topology(topology_options);

    shared_holder<generator> _gen;
    std::vector<mesh_triangle> _triangles;
    flat_map<std::tuple<unsigned, unsigned>, mesh_edge> _edges;
};
//------------------------------------------------------------------------------
} // namespace eagine::shapes

