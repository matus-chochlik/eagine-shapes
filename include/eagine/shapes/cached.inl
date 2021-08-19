/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
template <typename T>
void cached_gen::_get_values(
  const vertex_attrib_variant vav,
  span<T> dest,
  std::map<vertex_attrib_variant, std::vector<T>>& cache) {
    auto& cached = cache[vav];
    if(cached.empty()) {
        cached.resize(std_size(value_count(vav)));
        delegated_gen::attrib_values(vav, cover(cached));
    }
    copy(view(cached), dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _get_values(vav, dest, _byte_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _get_values(vav, dest, _int16_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _get_values(vav, dest, _uint16_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _get_values(vav, dest, _int32_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _get_values(vav, dest, _uint32_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _get_values(vav, dest, _float_cache);
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine
