/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module eagine.shapes;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.identifier;
import eagine.core.reflection;
import eagine.core.math;
import eagine.core.utility;
import eagine.core.runtime;
import eagine.core.logging;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
auto parse_from(const url& locator, generator&, to_json_options& opts) noexcept
  -> bool {
    opts.attrib_variants[vertex_attrib_kind::position][0];
    const auto& q{locator.query()};
    for(const auto& info : enumerator_mapping(
          std::type_identity<vertex_attrib_kind>(), default_selector)) {
        if(const auto var{q.arg_value_as<int>(info.name)}) {
            opts.attrib_variants[info.enumerator][*var];
        } else if(q.arg_has_value(info.name, true)) {
            opts.attrib_variants[info.enumerator][0];
        }
    }
    if(const auto var{q.arg_value_as<span_size_t>("draw")}) {
        opts.draw_variant = *var;
    }
    return true;
}
//------------------------------------------------------------------------------
auto parse_from(main_ctx& ctx, generator&, to_json_options& opts) noexcept
  -> bool {
    opts.attrib_variants[vertex_attrib_kind::position][0];
    for(const auto arg : ctx.args()) {
        if(arg.is_long_tag("shape-draw-variant")) {
            if(not assign_if_fits(arg.next(), opts.draw_variant)) {
                return false;
            }
        } else {
            for(const auto& info : enumerator_mapping(
                  std::type_identity<vertex_attrib_kind>(), default_selector)) {
                if(arg.is_prefixed_tag("--shape-attrib-", info.name)) {
                    if(arg.next().starts_with("-") or not arg.next()) {
                        opts.attrib_variants[info.enumerator][0];
                    } else {
                        std::int16_t var_idx{-1};
                        if(assign_if_fits(arg.next(), var_idx)) {
                            opts.attrib_variants[info.enumerator][var_idx];
                        } else {
                            ctx.log()
                              .error("failed to parse attribute index")
                              .arg("attrib", info.name)
                              .arg("value", arg.next().get());
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
auto to_json(std::ostream& out, generator& gen, const to_json_options& opts)
  -> std::ostream& {
    out << R"({"vertex_count":)" << gen.vertex_count() << '\n';

    for(const auto& [attr, variant] : opts.attrib_variants) {
        out << R"(,")" << enumerator_name(attr) << '"' << ":[\n{";
        interleaved_call separate([] {}, [&] { out << "},\n{"; });

        for(const auto& [index, name] : variant) {
            separate();
            const vertex_attrib_variant vav{attr, index};
            out << R"("values_per_vertex":)" << gen.values_per_vertex(vav)
                << '\n';

            const auto data_type = gen.attrib_type(vav);
            out << R"(,"type":")"
                << enumerator_name(
                     data_type,
                     std::type_identity<attrib_data_type>(),
                     from_value_tree_t{})
                << '"' << '\n';

            const auto size = gen.value_count(vav);
            out << R"(,"size":)" << size << '\n';

            if(not name.empty()) {
                out << R"(,"name":")" << name << '"' << '\n';
            }

            out << R"(,"data":[)";
            const auto print_data{[&out, &gen, vav, size](auto& data) {
                data.resize(integer(size));
                gen.attrib_values(vav, cover(data));

                interleaved_call print_elem(
                  [&out](const auto v) { out << v; }, [&out] { out << ','; });
                for(const auto v : data) {
                    print_elem(v);
                }
            }};

            if(data_type == attrib_data_type::float_) {
                std::vector<float> data;
                print_data(data);
            }
            out << "]\n";
        }
        out << "}]\n";
    }

    const auto idx_type = gen.index_type(opts.draw_variant);
    out << R"(,"index_type":")" << enumerator_name(idx_type) << '"' << '\n';

    if(idx_type != index_data_type::none) {
        std::vector<std::uint32_t> indices;
        indices.resize(integer(gen.index_count(opts.draw_variant)));
        if(not indices.empty()) {
            gen.indices(opts.draw_variant, cover(indices));
            out << R"(,"indices":[)";
            interleaved_call print_idx(
              [&out](const auto i) { out << i; }, [&out] { out << ','; });
            for(const auto i : indices) {
                print_idx(i);
            }
            out << "]\n";
        }
    }

    std::vector<draw_operation> operations;
    operations.resize(integer(gen.operation_count(opts.draw_variant)));
    if(not operations.empty()) {
        gen.instructions(opts.draw_variant, cover(operations));
        out << R"(,"instructions":[{)";

        const draw_operation cmp{};
        const auto should_print_phase = any_of(
          view(operations),
          [cmp](const auto& op) { return op.phase != cmp.phase; });
        const auto should_print_index_type = any_of(
          view(operations),
          [cmp](const auto& op) { return op.idx_type != cmp.idx_type; });
        const auto should_print_pr =
          any_of(view(operations), [cmp](const auto& op) {
              return op.primitive_restart != cmp.primitive_restart;
          });
        const auto should_print_pri =
          any_of(view(operations), [cmp](const auto& op) {
              return op.primitive_restart_index != cmp.primitive_restart_index;
          });
        const auto should_print_patch_verts =
          any_of(view(operations), [cmp](const auto& op) {
              return op.patch_vertices != cmp.patch_vertices;
          });

        interleaved_call print_op(
          [&out,
           should_print_phase,
           should_print_index_type,
           should_print_pr,
           should_print_pri,
           should_print_patch_verts](const auto& op) {
              out << R"("mode":")" << enumerator_name(op.mode) << '"';
              if(should_print_phase) {
                  out << R"(,"phase":)" << op.phase;
              }
              out << R"(,"first":)" << op.first;
              out << R"(,"count":)" << op.count;
              if(should_print_index_type) {
                  out << R"(,"index_type":")" << enumerator_name(op.idx_type)
                      << '"';
              }
              if(should_print_pr) {
                  out << R"(,"primitive_restart":)"
                      << (op.primitive_restart ? "true" : "false");
              }
              if(should_print_pri) {
                  out << R"(,"primitive_restart_index":)"
                      << op.primitive_restart_index;
              }
              if(should_print_patch_verts) {
                  out << R"(,"patch_vertices":)" << op.patch_vertices;
              }
              out << R"(,"cw_face_winding":)"
                  << (op.cw_face_winding ? "true" : "false");
          },
          [&out] { out << "},{"; });
        for(const auto& op : operations) {
            print_op(op);
        }
        out << "}]\n";
    }
    out << "}";
    return out;
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes
