/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///

module eagine.shapes;

import std;
import eagine.core.types;
import eagine.core.math;
import eagine.core.reflection;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto attrib_index(const vertex_attrib_kind attr) noexcept -> span_size_t {
    return enumerator_index(attr);
}
//------------------------------------------------------------------------------
auto attrib_index(const vertex_attrib_kind attr, span_size_t index) noexcept
  -> span_size_t {
    return attrib_index(attr) +
           index * enumerator_count(std::type_identity<vertex_attrib_kind>{});
}
//------------------------------------------------------------------------------
auto attrib_values_per_vertex(const vertex_attrib_kind attr) noexcept
  -> span_size_t {
    switch(attr) {
        case vertex_attrib_kind::color:
            return 4;
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::inner_position:
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
        case vertex_attrib_kind::roughness:
        case vertex_attrib_kind::pointiness:
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
auto default_attrib_value(const vertex_attrib_kind attr) noexcept
  -> vertex_attrib_value {
    switch(attr) {
        case vertex_attrib_kind::color:
            return math::vector<float, 4, true>{0.5F, 0.5F, 0.5F, 1.F};
        case vertex_attrib_kind::position:
        case vertex_attrib_kind::inner_position:
        case vertex_attrib_kind::pivot:
        case vertex_attrib_kind::pivot_pivot:
        case vertex_attrib_kind::vertex_pivot:
        case vertex_attrib_kind::vertex_coord:
        case vertex_attrib_kind::box_coord:
        case vertex_attrib_kind::face_coord:
        case vertex_attrib_kind::instance_offset:
        case vertex_attrib_kind::vector_field:
            return math::vector<float, 3, true>{};
        case vertex_attrib_kind::tangent:
            return math::vector<float, 3, true>{1.F, 0.F, 0.F};
        case vertex_attrib_kind::bitangent:
            return math::vector<float, 3, true>{0.F, 1.F, 0.F};
        case vertex_attrib_kind::normal:
            return math::vector<float, 3, true>{0.F, 0.F, 1.F};
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::tile_coord:
            return math::vector<float, 2, true>{};
        case vertex_attrib_kind::edge_length:
        case vertex_attrib_kind::opposite_length:
        case vertex_attrib_kind::face_area:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::scalar_field:
        case vertex_attrib_kind::roughness:
        case vertex_attrib_kind::occlusion:
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::instance_scale:
            return 1.F;
        case vertex_attrib_kind::pointiness:
            return 0.5F;
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
            return 0;
        case vertex_attrib_kind::instance_transform:
            break;
    }
    return {};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
