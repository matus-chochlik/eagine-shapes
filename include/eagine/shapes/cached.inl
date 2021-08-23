/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

namespace eagine::shapes {
//------------------------------------------------------------------------------
template <typename T>
void cached_gen::_get_values(
  const vertex_attrib_variant vav,
  span<T> dest,
  std::map<vertex_attrib_variant, std::vector<T>>& cache) {
    const auto& src = [this, vav, &cache]() -> const std::vector<T>& {
        std::lock_guard<std::mutex> lock{_mutex};
        auto& cached = cache[vav];
        if(cached.empty()) {
            cached.resize(std_size(value_count(vav)));
            delegated_gen::attrib_values(vav, cover(cached));
        }
        return cached;
    }();
    copy(view(src), dest);
}
//------------------------------------------------------------------------------
template <typename T>
void cached_gen::_get_indices(
  const drawing_variant var,
  span<T> dest,
  std::map<drawing_variant, std::vector<T>>& cache) {
    const auto& src = [this, var, &cache]() -> const std::vector<T>& {
        std::lock_guard<std::mutex> lock{_mutex};
        auto& cached = cache[var];
        if(cached.empty()) {
            cached.resize(std_size(index_count(var)));
            delegated_gen::indices(var, cover(cached));
        }
        return cached;
    }();
    copy(view(src), dest);
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
EAGINE_LIB_FUNC
void cached_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _get_indices(var, dest, _idx8_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    _get_indices(var, dest, _idx16_cache);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void cached_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    _get_indices(var, dest, _idx32_cache);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
