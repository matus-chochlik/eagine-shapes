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
//------------------------------------------------------------------------------
auto enumerator_traits<shapes::primitive_type>::mapping() noexcept
  -> enumerator_map_type<shapes::primitive_type, 11> {
    using shapes::primitive_type;
    return enumerator_map_type<primitive_type, 11>{
      {{"points", primitive_type::points},
       {"lines", primitive_type::lines},
       {"line_strip", primitive_type::line_strip},
       {"line_loop", primitive_type::line_loop},
       {"triangles", primitive_type::triangles},
       {"triangle_strip", primitive_type::triangle_strip},
       {"triangle_fan", primitive_type::triangle_fan},
       {"triangles_adjacency", primitive_type::triangles_adjacency},
       {"quads", primitive_type::quads},
       {"tetrahedrons", primitive_type::tetrahedrons},
       {"patches", primitive_type::patches}}};
}
//------------------------------------------------------------------------------
auto enumerator_traits<shapes::attrib_data_type>::mapping() noexcept
  -> enumerator_map_type<shapes::attrib_data_type, 7> {
    using shapes::attrib_data_type;
    return enumerator_map_type<attrib_data_type, 7>{
      {{"none", attrib_data_type::none},
       {"ubyte", attrib_data_type::ubyte},
       {"int_16", attrib_data_type::int_16},
       {"int_32", attrib_data_type::int_32},
       {"uint_16", attrib_data_type::uint_16},
       {"uint_32", attrib_data_type::uint_32},
       {"float_", attrib_data_type::float_}}};
}
//------------------------------------------------------------------------------
auto enumerator_traits<shapes::index_data_type>::mapping() noexcept
  -> enumerator_map_type<shapes::index_data_type, 4> {
    using shapes::index_data_type;
    return enumerator_map_type<index_data_type, 4>{
      {{"none", index_data_type::none},
       {"unsigned_8", index_data_type::unsigned_8},
       {"unsigned_16", index_data_type::unsigned_16},
       {"unsigned_32", index_data_type::unsigned_32}}};
}
//------------------------------------------------------------------------------
} // namespace eagine

