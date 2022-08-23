/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.reflection;
import eagine.core.math;
import eagine.core.value_tree;
import eagine.core.logging;
import eagine.core.main_ctx;
import <cmath>;
import <optional>;
import <string>;
import <map>;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class value_tree_loader
  : public main_ctx_object
  , public generator_base {
public:
    value_tree_loader(valtree::compound source, main_ctx_parent) noexcept;

    auto vertex_count() -> span_size_t override;

    auto attribute_variants(const vertex_attrib_kind) -> span_size_t override;

    auto variant_name(const vertex_attrib_variant) -> string_view override;

    auto values_per_vertex(const vertex_attrib_variant) -> span_size_t override;

    auto attrib_type(const vertex_attrib_variant vav)
      -> attrib_data_type override;

    auto is_attrib_normalized(const vertex_attrib_variant) -> bool override;

    void attrib_values(const vertex_attrib_variant, span<byte>) override;
    void attrib_values(const vertex_attrib_variant, span<std::int16_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::int32_t>) override;
    void attrib_values(const vertex_attrib_variant, span<std::uint16_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<std::uint32_t>)
      override;
    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto index_type(const drawing_variant) -> index_data_type override;

    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint16_t> dest) override;
    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    using _base = generator_base;

    valtree::compound _source{};
    std::string _temp{};
    std::map<vertex_attrib_variant, std::string> _variant_names{};

    static auto _attr_mask(const valtree::compound&) noexcept
      -> vertex_attrib_kinds;

    template <typename T>
    void _attrib_values(const vertex_attrib_variant, span<T>);
};
//------------------------------------------------------------------------------
auto vertex_attrib_name(const vertex_attrib_kind attrib) noexcept {
    return enumerator_name(
      attrib, std::type_identity<vertex_attrib_kind>{}, from_value_tree_t{});
}
//------------------------------------------------------------------------------
auto primitive_type_from(const string_view str) noexcept {
    return from_string(
      str, std::type_identity<primitive_type>{}, from_value_tree_t{});
}
//------------------------------------------------------------------------------
auto attrib_data_type_from(const string_view str) noexcept {
    return from_string(
      str, std::type_identity<attrib_data_type>{}, from_value_tree_t{});
}
//------------------------------------------------------------------------------
auto index_data_type_from(const string_view str) noexcept {
    return from_string(
      str, std::type_identity<index_data_type>{}, from_value_tree_t{});
}
//------------------------------------------------------------------------------
auto value_tree_loader::_attr_mask(const valtree::compound& source) noexcept
  -> vertex_attrib_kinds {
    vertex_attrib_kinds result;
    for(const auto& info : enumerator_mapping(
          std::type_identity<vertex_attrib_kind>{}, from_value_tree_t{})) {
        if(source.nested(info.name)) {
            result.set(info.enumerator);
        }
    }
    return result;
}
//------------------------------------------------------------------------------
value_tree_loader::value_tree_loader(
  valtree::compound source,
  main_ctx_parent parent) noexcept
  : main_ctx_object{"ValTreLoad", parent}
  , _base{_attr_mask(source), all_generator_capabilities()}
  , _source{std::move(source)} {}
//------------------------------------------------------------------------------
auto value_tree_loader::vertex_count() -> span_size_t {
    span_size_t result{};
    if(const auto count_a{_source.nested("vertex_count")}) {
        if(_source.fetch_value(count_a, result)) {
            return result;
        } else {
            log_error("could not fetch shape vertex count");
        }
    } else {
        log_error("could not find shape vertex count attribute");
    }
    return 0;
}
//------------------------------------------------------------------------------
auto value_tree_loader::attribute_variants(const vertex_attrib_kind attrib)
  -> span_size_t {
    if(const auto attrib_a{_source.nested(vertex_attrib_name(attrib))}) {
        return _source.nested_count(attrib_a);
    } else {
        log_error("could not query vertex attribute variant count")
          .arg("attribute", attrib);
    }
    return 0;
}
//------------------------------------------------------------------------------
auto value_tree_loader::variant_name(const vertex_attrib_variant vav)
  -> string_view {
    if(const auto attrib_a{
         _source.nested(vertex_attrib_name(vav.attribute()))}) {
        if(const auto variant_a{_source.nested(attrib_a, vav.index())}) {
            if(const auto vpv_a{_source.nested(variant_a, "name")}) {
                auto pos = _variant_names.find(vav);
                if(pos != _variant_names.end()) {
                    return {std::get<1>(*pos)};
                }
                auto& name = _variant_names[vav] = {};
                if(_source.fetch_value(vpv_a, name)) {
                    log_debug("cached attribute variant name")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index())
                      .arg("name", name);
                    return {name};
                } else {
                    log_error("could not fetch attribute variant name")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            }
        } else {
            log_error("could not find vertex attribute variant")
              .arg("attribute", vav.attribute())
              .arg("variant", vav.index());
        }
    } else {
        log_error("could not find vertex attribute")
          .arg("attribute", vav.attribute());
    }
    return {};
}
//------------------------------------------------------------------------------
auto value_tree_loader::values_per_vertex(const vertex_attrib_variant vav)
  -> span_size_t {
    if(const auto attrib_a{
         _source.nested(vertex_attrib_name(vav.attribute()))}) {
        if(const auto variant_a{_source.nested(attrib_a, vav.index())}) {
            if(const auto vpv_a{
                 _source.nested(variant_a, "values_per_vertex")}) {
                span_size_t result{};
                if(_source.fetch_value(vpv_a, result)) {
                    return result;
                } else {
                    log_error("could not fetch attribute variant vertex count")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            }
        } else {
            log_error("could not find vertex attribute variant")
              .arg("attribute", vav.attribute())
              .arg("variant", vav.index());
        }
    } else {
        log_error("could not find vertex attribute")
          .arg("attribute", vav.attribute());
    }
    return _base::values_per_vertex(vav);
}
//------------------------------------------------------------------------------
auto value_tree_loader::attrib_type(const vertex_attrib_variant vav)
  -> attrib_data_type {
    if(const auto attrib_a{
         _source.nested(vertex_attrib_name(vav.attribute()))}) {
        if(const auto variant_a{_source.nested(attrib_a, vav.index())}) {
            if(const auto type_a{_source.nested(variant_a, "type")}) {
                if(_source.fetch_value(type_a, _temp)) {
                    if(const auto type{attrib_data_type_from(view(_temp))}) {
                        return extract(type);
                    } else {
                        log_error("unknown attribute variant data type")
                          .arg("type", _temp)
                          .arg("attribute", vav.attribute())
                          .arg("variant", vav.index());
                    }
                } else {
                    log_error("could not fetch attribute variant data type")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            }
        } else {
            log_error("could not find vertex attribute variant")
              .arg("attribute", vav.attribute())
              .arg("variant", vav.index());
        }
    } else {
        log_error("could not find vertex attribute")
          .arg("attribute", vav.attribute());
    }
    return attrib_data_type::float_;
}
//------------------------------------------------------------------------------
auto value_tree_loader::is_attrib_normalized(const vertex_attrib_variant vav)
  -> bool {
    if(const auto attrib_a{
         _source.nested(vertex_attrib_name(vav.attribute()))}) {
        if(const auto variant_a{_source.nested(attrib_a, vav.index())}) {
            if(const auto norm_a{_source.nested(variant_a, "normalized")}) {
                bool normalized{false};
                if(_source.fetch_value(norm_a, normalized)) {
                    return normalized;
                } else {
                    log_error("could not fetch attribute variant normalized")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            } else if(const auto type_a{_source.nested(variant_a, "type")}) {
                if(_source.fetch_value(type_a, _temp)) {
                    if(const auto type{attrib_data_type_from(view(_temp))}) {
                        return extract(type) != attrib_data_type::float_;
                    } else {
                        log_error("unknown attribute variant data type")
                          .arg("type", _temp)
                          .arg("attribute", vav.attribute())
                          .arg("variant", vav.index());
                    }
                } else {
                    log_error("could not fetch attribute variant data type")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            }

        } else {
            log_error("could not find vertex attribute variant")
              .arg("attribute", vav.attribute())
              .arg("variant", vav.index());
        }
    } else {
        log_error("could not find vertex attribute")
          .arg("attribute", vav.attribute());
    }
    return false;
}
//------------------------------------------------------------------------------
template <typename T>
void value_tree_loader::_attrib_values(
  const vertex_attrib_variant vav,
  span<T> dest) {
    if(const auto attrib_a{
         _source.nested(vertex_attrib_name(vav.attribute()))}) {
        if(const auto variant_a{_source.nested(attrib_a, vav.index())}) {
            if(const auto data_a{_source.nested(variant_a, "data")}) {
                if(_source.fetch_values(data_a, dest)) {
                    log_debug("loaded vertex attribute data")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                } else {
                    log_error("could not fetch vertex attribute data")
                      .arg("attribute", vav.attribute())
                      .arg("variant", vav.index());
                }
            } else {
                log_error("could not find vertex attribute data")
                  .arg("attribute", vav.attribute())
                  .arg("variant", vav.index());
            }
        } else {
            log_error("could not find vertex attribute variant")
              .arg("attribute", vav.attribute())
              .arg("variant", vav.index());
        }
    } else {
        log_error("could not find vertex attribute")
          .arg("attribute", vav.attribute());
    }
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<byte> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<std::int32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint16_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<std::uint32_t> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
void value_tree_loader::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    _attrib_values(vav, dest);
}
//------------------------------------------------------------------------------
auto value_tree_loader::index_type(const drawing_variant) -> index_data_type {
    if(const auto type_a{_source.nested("index_type")}) {
        if(_source.fetch_value(type_a, _temp)) {
            if(const auto type{index_data_type_from(view(_temp))}) {
                return extract(type);
            } else {
                log_error("unsupported index type value").arg("type", _temp);
            }
        } else {
            log_error("could not fetch index type value");
        }
    }
    return index_data_type::none;
}
//------------------------------------------------------------------------------
auto value_tree_loader::index_count(const drawing_variant) -> span_size_t {
    if(const auto indices_a{_source.nested("indices")}) {
        return _source.value_count(indices_a);
    }
    return 0;
}
//------------------------------------------------------------------------------
void value_tree_loader::indices(
  const drawing_variant,
  span<std::uint16_t> dest) {
    if(const auto indices_a{_source.nested("indices")}) {
        if(_source.fetch_values(indices_a, dest)) {
            log_debug("loaded indices");
        } else {
            log_error("could not fetch shape 32-bit indices");
        }
    }
}
//------------------------------------------------------------------------------
void value_tree_loader::indices(
  const drawing_variant,
  span<std::uint32_t> dest) {
    if(const auto indices_a{_source.nested("indices")}) {
        if(_source.fetch_values(indices_a, dest)) {
            log_debug("loaded indices").arg("indices", view(dest));
        } else {
            log_error("could not fetch shape 32-bit indices");
        }
    }
}
//------------------------------------------------------------------------------
auto value_tree_loader::operation_count(const drawing_variant) -> span_size_t {
    if(const auto instrs_a{_source.nested("instructions")}) {
        return _source.nested_count(instrs_a);
    } else {
        log_error("could not query shape draw instructions count");
    }
    return 0;
}
//------------------------------------------------------------------------------
void value_tree_loader::instructions(
  const drawing_variant,
  span<draw_operation> ops) {
    bool btemp{};
    if(const auto instrs_a{_source.nested("instructions")}) {
        if(ops.size() == _source.nested_count(instrs_a)) {
            for(const auto i : integer_range(ops.size())) {
                if(const auto instr_a{_source.nested(instrs_a, i)}) {
                    auto& op = ops[i];

                    // first
                    if(const auto first_a{_source.nested(instr_a, "first")}) {
                        if(!_source.fetch_value(first_a, op.first)) {
                            log_error("could not fetch shape draw offset")
                              .arg("index", i);
                        }
                    } else {
                        log_error("could not find shape draw offset")
                          .arg("index", i);
                    }

                    // count
                    if(const auto count_a{_source.nested(instr_a, "count")}) {
                        if(!_source.fetch_value(count_a, op.count)) {
                            log_error("could not fetch shape draw count")
                              .arg("index", i);
                        }
                    } else {
                        log_error("could not find shape draw count")
                          .arg("index", i);
                    }

                    // phase
                    if(const auto phase_a{_source.nested(instr_a, "phase")}) {
                        if(!_source.fetch_value(phase_a, op.phase)) {
                            log_error("could not fetch draw phase value")
                              .arg("index", i);
                        }
                    }

                    // primitive restart index
                    if(const auto pri_a{
                         _source.nested(instr_a, "primitive_restart_index")}) {
                        if(!_source.fetch_value(
                             pri_a, op.primitive_restart_index)) {
                            log_error("could not fetch restart index")
                              .arg("index", i);
                        }
                    }

                    // patch_vertices
                    if(const auto ptch_vert_a{
                         _source.nested(instr_a, "patch_vertices")}) {
                        if(!_source.fetch_value(
                             ptch_vert_a, op.patch_vertices)) {
                            log_error("could not fetch patch vertex count")
                              .arg("index", i);
                        }
                    }

                    // draw mode
                    if(const auto mode_a{_source.nested(instr_a, "mode")}) {
                        if(_source.fetch_value(mode_a, _temp)) {
                            if(const auto mode{
                                 primitive_type_from(view(_temp))}) {
                                op.mode = extract(mode);
                            } else {
                                log_error("unsupported shape draw mode")
                                  .arg("mode", _temp)
                                  .arg("index", i);
                            }
                        } else {
                            log_error("could not fetch shape draw mode")
                              .arg("index", i);
                        }
                    } else {
                        log_error("could not find shape draw mode")
                          .arg("index", i);
                    }

                    // index type
                    if(const auto type_a{
                         _source.nested(instr_a, "index_type")}) {
                        if(_source.fetch_value(type_a, _temp)) {
                            if(const auto type{
                                 index_data_type_from(view(_temp))}) {
                                op.idx_type = extract(type);
                            } else {
                                log_error("unsupported index type value")
                                  .arg("type", _temp);
                            }
                        } else {
                            log_error("could not fetch index type value");
                        }
                    }

                    // primitive restart
                    if(const auto prirest_a{
                         _source.nested(instr_a, "primitive_restart")}) {
                        if(!_source.fetch_value(prirest_a, btemp)) {
                            op.primitive_restart = btemp;
                        } else {
                            log_error("invalid primitive restart flag")
                              .arg("index", i);
                        }
                    }

                    // face winding direction
                    if(const auto cwfw_a{
                         _source.nested(instr_a, "cw_face_winding")}) {
                        if(_source.fetch_value(cwfw_a, btemp)) {
                            op.cw_face_winding = btemp;
                        } else {
                            log_error("invalid face winding direction")
                              .arg("index", i);
                        }
                    }
                    log_debug("loaded draw operation")
                      .arg("first", op.first)
                      .arg("count", op.count)
                      .arg("phase", op.phase)
                      .arg("prmRstrIdx", op.primitive_restart_index)
                      .arg("mode", op.mode)
                      .arg("indexType", op.idx_type)
                      .arg("primRstrt", bool(op.primitive_restart))
                      .arg("cwFaceWndg", bool(op.cw_face_winding));
                } else {
                    log_error("could not get shape draw instruction")
                      .arg("index", i);
                }
            }
        } else {
            log_error("shape draw instructions destination size mismatch")
              .arg("dest", ops.size())
              .arg("actual", _source.nested_count(instrs_a));
        }
    } else {
        log_error("could not find shape draw instructions");
    }
}
//------------------------------------------------------------------------------
auto from_value_tree(valtree::compound source, main_ctx_parent parent)
  -> std::unique_ptr<generator> {
    return std::make_unique<value_tree_loader>(std::move(source), parent);
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
