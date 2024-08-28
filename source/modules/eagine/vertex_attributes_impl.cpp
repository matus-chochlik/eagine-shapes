/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///

module eagine.shapes;

import std;
import eagine.core;

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
auto attrib_values_per_vertex(const vertex_attrib_kind attr) noexcept
  -> span_size_t {
    switch(attr) {
        case vertex_attrib_kind::color:
        case vertex_attrib_kind::emission:
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
            return math::vector<float, 4>{0.5F, 0.5F, 0.5F, 1.F};
        case vertex_attrib_kind::emission:
            return math::vector<float, 4>{0.0F, 0.0F, 0.0F, 0.F};
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
            return math::vector<float, 3>{};
        case vertex_attrib_kind::tangent:
            return math::vector<float, 3>{1.F, 0.F, 0.F};
        case vertex_attrib_kind::bitangent:
            return math::vector<float, 3>{0.F, 1.F, 0.F};
        case vertex_attrib_kind::normal:
            return math::vector<float, 3>{0.F, 0.F, 1.F};
        case vertex_attrib_kind::wrap_coord:
        case vertex_attrib_kind::tile_coord:
            return math::vector<float, 2>{};
        case vertex_attrib_kind::edge_length:
        case vertex_attrib_kind::opposite_length:
        case vertex_attrib_kind::face_area:
        case vertex_attrib_kind::weight:
        case vertex_attrib_kind::occlusion:
        case vertex_attrib_kind::instance_scale:
            return 1.F;
        case vertex_attrib_kind::roughness:
            return 0.8F;
        case vertex_attrib_kind::scalar_field:
        case vertex_attrib_kind::pointiness:
            return 0.5F;
        case vertex_attrib_kind::object_id:
        case vertex_attrib_kind::polygon_id:
        case vertex_attrib_kind::material_id:
            return 0;
        case vertex_attrib_kind::instance_transform:
            break;
    }
    return {};
}
//------------------------------------------------------------------------------
shared_vertex_attrib_variants::shared_vertex_attrib_variants(
  std::initializer_list<vertex_attrib_variant> vavs) noexcept
  : base_ptr{std::make_shared<vertex_attrib_variant[]>(vavs.size())}
  , base_span{get(), span_size(vavs.size())} {
    copy(
      eagine::memory::view(vavs),
      memory::span<vertex_attrib_variant>{get(), span_size(vavs.size())});
}
//------------------------------------------------------------------------------
auto shared_vertex_attrib_variants::basic() noexcept
  -> const shared_vertex_attrib_variants& {
    static const shared_vertex_attrib_variants result{
      {vertex_attrib_kind::position,
       vertex_attrib_kind::pivot,
       vertex_attrib_kind::vertex_pivot,
       vertex_attrib_kind::normal,
       vertex_attrib_kind::tangent,
       vertex_attrib_kind::bitangent,
       vertex_attrib_kind::color,
       vertex_attrib_kind::emission,
       vertex_attrib_kind::vertex_coord,
       vertex_attrib_kind::wrap_coord,
       vertex_attrib_kind::face_coord,
       vertex_attrib_kind::tile_coord,
       vertex_attrib_kind::box_coord,
       vertex_attrib_kind::roughness,
       vertex_attrib_kind::pointiness,
       vertex_attrib_kind::occlusion}};
    return result;
}
//------------------------------------------------------------------------------
} // namespace shapes
auto enumerator_traits<shapes::vertex_attrib_kind>::mapping() noexcept
  -> enumerator_map_type<shapes::vertex_attrib_kind, 30> {
    using shapes::vertex_attrib_kind;
    return enumerator_map_type<vertex_attrib_kind, 30>{
      {{"position", vertex_attrib_kind::position},
       {"normal", vertex_attrib_kind::normal},
       {"tangent", vertex_attrib_kind::tangent},
       {"bitangent", vertex_attrib_kind::bitangent},
       {"wrap_coord", vertex_attrib_kind::wrap_coord},
       {"tile_coord", vertex_attrib_kind::tile_coord},
       {"vertex_coord", vertex_attrib_kind::vertex_coord},
       {"vertex_pivot", vertex_attrib_kind::vertex_pivot},
       {"pivot_pivot", vertex_attrib_kind::pivot_pivot},
       {"inner_position", vertex_attrib_kind::inner_position},
       {"color", vertex_attrib_kind::color},
       {"emission", vertex_attrib_kind::emission},
       {"roughness", vertex_attrib_kind::roughness},
       {"occlusion", vertex_attrib_kind::occlusion},
       {"pointiness", vertex_attrib_kind::pointiness},
       {"weight", vertex_attrib_kind::weight},
       {"pivot", vertex_attrib_kind::pivot},
       {"box_coord", vertex_attrib_kind::box_coord},
       {"face_coord", vertex_attrib_kind::face_coord},
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
