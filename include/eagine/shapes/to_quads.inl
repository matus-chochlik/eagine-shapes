/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/assert.hpp>
#include <eagine/math/functions.hpp>
#include <eagine/memory/span_algo.hpp>
#include <vector>

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto to_quads_gen::index_count(const drawing_variant var) -> span_size_t {

    span_size_t count{0};

    std::vector<draw_operation> ops;
    ops.resize(std_size(delegated_gen::operation_count(var)));
    delegated_gen::instructions(var, cover(ops));

    std::vector<std::uint32_t> idx;
    idx.resize(std_size(delegated_gen::index_count(var)));
    delegated_gen::indices(var, cover(idx));

    const auto num_verts = [](auto n) {
        return n > 0 ? span_size(((n / 2) - 1) * 4) : 0;
    };

    for(const auto& op : ops) {
        if(op.mode == primitive_type::triangle_strip) {
            if(op.idx_type == index_data_type::none) {
                count += num_verts(op.count);
            } else {
                span_size_t prev{0};
                span_size_t curr{0};
                if(op.primitive_restart && delegated_gen::primitive_restart()) {
                    while(curr < op.count) {
                        if(
                          idx[std_size(op.first + curr)] ==
                          op.primitive_restart_index) {
                            count += num_verts(curr - prev);
                            prev = curr + 1;
                        }
                        ++curr;
                    }
                } else {
                    curr = op.count;
                }
                count += num_verts(curr - prev);
            }
        } else {
            count += op.count;
        }
    }

    return count;
}
//------------------------------------------------------------------------------
template <typename T>
void to_quads_gen::_indices(const drawing_variant var, span<T> dest) noexcept {

    EAGINE_ASSERT(dest.size() >= index_count(var));

    span_size_t i = 0;

    std::vector<draw_operation> ops;
    ops.resize(std_size(delegated_gen::operation_count(var)));
    delegated_gen::instructions(var, cover(ops));

    std::vector<std::uint32_t> del_idx;
    del_idx.resize(std_size(delegated_gen::index_count(var)));
    delegated_gen::indices(var, cover(del_idx));

    for(const auto& op : ops) {
        if(op.mode == primitive_type::triangle_strip) {
            if(op.idx_type == index_data_type::none) {
                for(const auto p : integer_range(1, op.count / 2)) {
                    dest[i++] = limit_cast<T>(op.first + p * 2 - 2);
                    dest[i++] = limit_cast<T>(op.first + p * 2 - 1);
                    dest[i++] = limit_cast<T>(op.first + p * 2 + 0);
                    dest[i++] = limit_cast<T>(op.first + p * 2 + 1);
                }
            } else {
                span_size_t curr{0};

                while(curr + 4 <= op.count) {
                    for(const auto t : integer_range(4)) {
                        if(
                          op.primitive_restart &&
                          delegated_gen::primitive_restart() &&
                          (del_idx[std_size(op.first + curr + t)] ==
                           op.primitive_restart_index)) {
                            curr += t + 1;
                            break;
                        }
                    }
                    if(curr + 4 <= op.count) {
                        dest[i++] =
                          limit_cast<T>(del_idx[std_size(op.first + curr + 0)]);
                        dest[i++] =
                          limit_cast<T>(del_idx[std_size(op.first + curr + 1)]);
                        dest[i++] =
                          limit_cast<T>(del_idx[std_size(op.first + curr + 2)]);
                        dest[i++] =
                          limit_cast<T>(del_idx[std_size(op.first + curr + 3)]);
                        curr += 2;
                    } else {
                        break;
                    }
                }
            }
        } else {
            if(op.idx_type == index_data_type::none) {
                for(const auto p : integer_range(op.count)) {
                    dest[i++] = limit_cast<T>(op.first + p);
                }
            } else {
                for(const auto p : integer_range(op.count)) {
                    dest[i++] = limit_cast<T>(del_idx[op.first + p]);
                }
            }
        }
    }

    EAGINE_ASSERT(i == index_count(var));
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void to_quads_gen::indices(const drawing_variant var, span<std::uint8_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void to_quads_gen::indices(const drawing_variant var, span<std::uint16_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void to_quads_gen::indices(const drawing_variant var, span<std::uint32_t> dest) {
    _indices(var, dest);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
auto to_quads_gen::operation_count(const drawing_variant var) -> span_size_t {
    return delegated_gen::operation_count(var);
}
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void to_quads_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {

    delegated_gen::instructions(var, ops);

    std::vector<std::uint32_t> idx;
    idx.resize(std_size(delegated_gen::index_count(var)));
    delegated_gen::indices(var, cover(idx));

    const auto it = index_type(var);

    const auto num_verts = [](auto n) {
        return n > 0 ? span_size(((n / 2) - 1) * 4) : 0;
    };

    span_size_t offs{0};

    for(auto& op : ops) {
        if(op.mode == primitive_type::triangle_strip) {
            op.mode = primitive_type::quads;
            span_size_t count{0};
            span_size_t prev{0};
            span_size_t curr{0};
            if(op.primitive_restart && delegated_gen::primitive_restart()) {
                while(curr < op.count) {
                    if(
                      idx[std_size(op.first + curr)] ==
                      op.primitive_restart_index) {
                        count += num_verts(curr - prev);
                        prev = curr + 1;
                    }
                    ++curr;
                }
            } else {
                curr = op.count;
            }

            count += num_verts(curr - prev);
            op.count = count;
            op.primitive_restart = false;
        }
        op.idx_type = it;
        op.first = offs;
        offs += op.count;
    }
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine
