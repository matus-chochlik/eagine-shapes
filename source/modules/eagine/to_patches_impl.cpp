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

import std;
import eagine.core.types;
import eagine.core.memory;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class to_patches_gen : public delegated_gen {

public:
    to_patches_gen(std::shared_ptr<generator> gen) noexcept
      : delegated_gen{std::move(gen)} {}

    auto index_type(const drawing_variant) -> index_data_type override;
    auto index_count(const drawing_variant) -> span_size_t override;

    void indices(const drawing_variant, span<std::uint8_t> dest) override;
    void indices(const drawing_variant, span<std::uint16_t> dest) override;
    void indices(const drawing_variant, span<std::uint32_t> dest) override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

private:
    struct _op_param {
        span_size_t first{0};
        span_size_t count{0};
    };

    struct _instr_info {
        std::vector<draw_operation> ops;
        std::vector<_op_param> params;
        bool needs_reindex{false};
    };

    auto _get_instr(const drawing_variant) -> _instr_info&;

    template <typename T>
    void _make_indices(const drawing_variant, std::vector<T>&);

    template <typename T>
    auto _get_indices(const drawing_variant) -> std::vector<T>&;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest);

    std::map<drawing_variant, _instr_info> _instructions;
    std::map<
      drawing_variant,
      std::variant<
        std::vector<std::uint8_t>,
        std::vector<std::uint16_t>,
        std::vector<std::uint32_t>>>
      _new_indices;
};
//------------------------------------------------------------------------------
auto to_patches(std::shared_ptr<generator> gen) noexcept
  -> std::unique_ptr<generator> {
    return std::make_unique<to_patches_gen>(std::move(gen));
}
//------------------------------------------------------------------------------
auto to_patches_gen::_get_instr(const drawing_variant var) -> _instr_info& {
    auto pos{_instructions.find(var)};
    if(pos == _instructions.end()) {
        pos = _instructions.emplace(var, _instr_info{}).first;
        auto& info = pos->second;
        info.ops.resize(std_size(delegated_gen::operation_count(var)));
        delegated_gen::instructions(var, cover(info.ops));
        info.needs_reindex = false;

        for(const auto& op : info.ops) {
            if(
              op.mode != primitive_type::triangles and
              op.mode != primitive_type::quads) {
                info.needs_reindex = true;
                break;
            }
        }
    }
    return pos->second;
}
//------------------------------------------------------------------------------
template <typename T>
void to_patches_gen::_make_indices(
  const drawing_variant var,
  std::vector<T>& new_indices) {

    std::vector<T> old_indices;
    old_indices.resize(std_size(delegated_gen::index_count(var)));
    delegated_gen::indices(var, cover(old_indices));

    auto& info{_get_instr(var)};

    for(const auto i : integer_range(info.ops.size())) {
        auto& op = info.ops[i];
        if(op.idx_type == index_data_type::none) {
        } else {
            if(op.mode == primitive_type::triangle_strip) {
                new_indices.reserve(
                  new_indices.size() + std_size((op.count - 1) * 5 / 2));

                info.params.reserve(info.ops.size());
                info.params.emplace_back();
                auto& params = info.params.back();
                params.first = span_size(new_indices.size());

                const auto chunk{
                  head(skip(view(old_indices), op.first), op.count)};

                T i2{};
                T i1{};
                bool r2{true};
                bool r1{true};
                bool even{true};
                for(const auto i0 : chunk) {
                    if(
                      op.primitive_restart and
                      (i0 == op.primitive_restart_index)) {
                        r2 = true;
                        r1 = true;
                        even = true;
                    } else {
                        if(r1 or r2) {
                            r2 = r1;
                            r1 = false;
                        } else {
                            if(even) {
                                new_indices.push_back(i2);
                                new_indices.push_back(i1);
                                new_indices.push_back(i0);
                            } else {
                                new_indices.push_back(i1);
                                new_indices.push_back(i2);
                                new_indices.push_back(i0);
                            }
                            even = not even;
                        }
                    }
                    i2 = i1;
                    i1 = i0;
                }
                params.count = span_size(new_indices.size()) - params.first;
            }
        }
    }
}
//------------------------------------------------------------------------------
template <typename T>
auto to_patches_gen::_get_indices(const drawing_variant var)
  -> std::vector<T>& {
    auto pos{_new_indices.find(var)};
    if(pos == _new_indices.end()) {
        pos = _new_indices.emplace(var, std::vector<T>{}).first;
        _make_indices(var, std::get<std::vector<T>>(pos->second));
    }
    return std::get<std::vector<T>>(pos->second);
}
//------------------------------------------------------------------------------
auto to_patches_gen::index_type(const drawing_variant var) -> index_data_type {
    auto old_type{delegated_gen::index_type(var)};
    if(old_type == index_data_type::none) {
        old_type = index_data_type::unsigned_32;
    }
    return old_type;
}
//------------------------------------------------------------------------------
auto to_patches_gen::index_count(const drawing_variant var) -> span_size_t {
    if(_get_instr(var).needs_reindex) {
        switch(delegated_gen::index_type(var)) {
            case index_data_type::unsigned_8:
                return span_size(_get_indices<std::uint8_t>(var).size());
            case index_data_type::unsigned_16:
                return span_size(_get_indices<std::uint16_t>(var).size());
            case index_data_type::unsigned_32:
            case index_data_type::none:
                return span_size(_get_indices<std::uint32_t>(var).size());
        }
    } else {
        return delegated_gen::index_count(var);
    }
}
//------------------------------------------------------------------------------
template <typename T>
void to_patches_gen::_indices(const drawing_variant var, span<T> dest) {
    if(_get_instr(var).needs_reindex) {
        memory::copy(view(_get_indices<T>(var)), dest);
    } else {
        delegated_gen::indices(var, dest);
    }
}
//------------------------------------------------------------------------------
void to_patches_gen::indices(
  const drawing_variant var,
  span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void to_patches_gen::indices(
  const drawing_variant var,
  span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void to_patches_gen::indices(
  const drawing_variant var,
  span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
void to_patches_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {

    // this generates additional info when instructions is called first
    (void)index_count(var);

    auto& info{_get_instr(var)};
    for(const auto i : integer_range(info.ops.size())) {
        auto& op = ops[i];
        op = info.ops[i];
        if(op.mode == primitive_type::triangles) {
            op.mode = primitive_type::patches;
            op.patch_vertices = 3;
        } else if(op.mode == primitive_type::triangle_strip) {
            assert(info.ops.size() == info.params.size());
            op.mode = primitive_type::patches;
            op.patch_vertices = 3;
            op.primitive_restart_index = 0;
            op.primitive_restart = false;
            op.first = info.params[i].first;
            op.count = info.params[i].count;
        } else if(op.mode == primitive_type::quads) {
            op.mode = primitive_type::patches;
            op.patch_vertices = 4;
        }
    }
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
