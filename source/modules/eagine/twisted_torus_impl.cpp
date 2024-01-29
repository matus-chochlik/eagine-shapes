/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
/// https://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;

import std;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.valid_if;
import eagine.core.math;
import eagine.core.runtime;
import eagine.core.main_ctx;

namespace eagine::shapes {
//------------------------------------------------------------------------------
class unit_twisted_torus_gen : public generator_base {
public:
    unit_twisted_torus_gen(
      const vertex_attrib_kinds attr_kinds,
      const int twist,
      const valid_if_greater_than<int, 2>& rings,
      const valid_if_greater_than<int, 3>& sections,
      const valid_if_ge0_lt1<float>& radius_ratio) noexcept;

    auto vertex_count() -> span_size_t override;

    void positions(span<float> dest) noexcept;

    void normals(span<float> dest) noexcept;

    void wrap_coords(span<float> dest) noexcept;

    void attrib_values(const vertex_attrib_variant, span<float>) override;

    auto operation_count(const drawing_variant) -> span_size_t override;

    void instructions(const drawing_variant, span<draw_operation> ops) override;

    auto bounding_sphere() -> math::sphere<float, true> override;

private:
    using _base = generator_base;

    int _twist;
    int _rings;
    int _sections;
    float _radius_ratio{0.50F};
    float _thickness_ratio{0.1F};

    static auto _attr_mask() noexcept -> vertex_attrib_kinds;

    template <typename T>
    void _indices(const drawing_variant, span<T> dest) noexcept;
};
//------------------------------------------------------------------------------
auto unit_twisted_torus_from(
  const vertex_attrib_kinds attr_kinds,
  const url& locator,
  main_ctx&) -> shared_holder<generator> {
    if(locator.has_path("/unit_twisted_torus")) {
        const auto twist{
          locator.query().arg_value_as("twist", std::type_identity<int>{})};
        const auto rings{locator.query().arg_value_as(
          "rings", std::type_identity<valid_if_greater_than<int, 2>>{})};
        const auto sections{locator.query().arg_value_as(
          "sections", std::type_identity<valid_if_greater_than<int, 3>>{})};
        const auto radius_ratio{locator.query().arg_value_as(
          "radius_ratio", std::type_identity<valid_if_ge0_lt1<float>>{})};
        return unit_twisted_torus(
          attr_kinds,
          twist.value_or(12),
          rings.value_or(38),
          sections.value_or(12),
          radius_ratio.value_or(0.5F));
    }
    return {};
}
//------------------------------------------------------------------------------
auto unit_twisted_torus(
  const vertex_attrib_kinds attr_kinds,
  const int twist,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) -> shared_holder<generator> {
    return {
      hold<unit_twisted_torus_gen>,
      attr_kinds,
      twist,
      rings,
      sections,
      radius_ratio};
}
//------------------------------------------------------------------------------
auto unit_twisted_torus_gen::_attr_mask() noexcept -> vertex_attrib_kinds {
    return vertex_attrib_kind::position | vertex_attrib_kind::normal |
           vertex_attrib_kind::box_coord | vertex_attrib_kind::wrap_coord;
}
//------------------------------------------------------------------------------
unit_twisted_torus_gen::unit_twisted_torus_gen(
  const vertex_attrib_kinds attr_kinds,
  const int twist,
  const valid_if_greater_than<int, 2>& rings,
  const valid_if_greater_than<int, 3>& sections,
  const valid_if_ge0_lt1<float>& radius_ratio) noexcept
  : _base{attr_kinds & _attr_mask(), generator_capability::element_strips}
  , _twist{twist}
  , _rings{rings.value()}
  , _sections{sections.value()}
  , _radius_ratio{radius_ratio.value()} {}
//------------------------------------------------------------------------------
auto unit_twisted_torus_gen::vertex_count() -> span_size_t {
    return 2 * 2 * 2 * _sections * (_rings + 1);
}
//------------------------------------------------------------------------------
void unit_twisted_torus_gen::positions(span<float> dest) noexcept {

    assert(has(vertex_attrib_kind::position));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const double ro = 0.25;
    const double ri = ro * _radius_ratio;
    const double r1 = ri;
    const double r2 = ro - ri;

    const double r_twist = double(_twist) / double(_rings);
    const double r_step = math::tau / double(_rings);
    const double s_step = math::tau / double(_sections);
    const double s_slip = s_step * _thickness_ratio * 2.0;

    for(const auto f : integer_range(2)) {
        const double f_sign = (f == 0) ? 1.0 : -1.0;
        const double fdt = s_slip * f_sign * 0.25;

        for(const auto s : integer_range(_sections)) {
            const double s_angle = s_step * 0.5 + s * s_step;
            const double sa[2] = {
              s_angle + s_slip * f_sign, s_angle - s_slip * f_sign};

            for(const auto r : integer_range(_rings + 1)) {
                const double r_angle = r * r_step;

                const double vx = std::cos(r_angle);
                const double vz = std::sin(r_angle);

                const double ta = s_step * r * r_twist;

                for(const auto d : integer_range(2)) {
                    double vr = std::cos(sa[d] + ta);
                    double vy = std::sin(sa[d] + ta);

                    dest[k++] = float(vx * (r1 + r2 * (1.0 + vr) + fdt * vr));
                    dest[k++] = float(vy * (r2 + fdt));
                    dest[k++] = float(vz * (r1 + r2 * (1.0 + vr) + fdt * vr));
                }
            }
        }
    }

    for(const auto d : integer_range(2)) {
        const double d_sign = (d == 0) ? 1.0 : -1.0;

        for(const auto s : integer_range(_sections)) {
            const double s_angle = s_step * 0.5 + s * s_step;
            const double sa = s_angle + s_slip * d_sign;

            for(const auto r : integer_range(_rings + 1)) {
                const double r_angle = r * r_step;
                const double ta = s_step * r * r_twist;
                const double vr = std::cos(sa + ta);
                const double vy = std::sin(sa + ta);

                const double vx = std::cos(r_angle);
                const double vz = std::sin(r_angle);

                for(unsigned f = 0; f != 2; ++f) {
                    const double f_sign = (f == 0) ? 1.0 : -1.0;
                    const double fdt = -s_slip * d_sign * f_sign * 0.25;

                    dest[k++] = float(vx * (r1 + r2 * (1.0 + vr) + fdt * vr));
                    dest[k++] = float(vy * (r2 + fdt));
                    dest[k++] = float(vz * (r1 + r2 * (1.0 + vr) + fdt * vr));
                }
            }
        }
    }

    assert(k <= dest.size());
}
//------------------------------------------------------------------------------
void unit_twisted_torus_gen::normals(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::normal));
    assert(dest.size() >= vertex_count() * 3);

    span_size_t k = 0;

    const double r_twist = double(_twist) / double(_rings);
    const double r_step = math::tau / double(_rings);
    const double s_step = math::tau / double(_sections);
    const double s_slip = s_step * _thickness_ratio * 2.0;

    for(const auto f : integer_range(2)) {
        const double f_sign = (f == 0) ? 1.0 : -1.0;

        for(const auto s : integer_range(_sections)) {
            const double s_angle = s_step * 0.5 + s * s_step;
            const double sa[2] = {
              s_angle + s_slip * f_sign, s_angle - s_slip * f_sign};

            for(const auto r : integer_range(_rings + 1)) {
                const double r_angle = r * r_step;
                double vx = std::cos(r_angle);
                double vz = std::sin(r_angle);

                const double ta = s_step * r * r_twist;

                for(const auto d : integer_range(2)) {
                    double vr = std::cos(sa[d] + ta);
                    double vy = std::sin(sa[d] + ta);

                    dest[k++] = float(f_sign * vx * vr);
                    dest[k++] = float(f_sign * vy);
                    dest[k++] = float(f_sign * vz * vr);
                }
            }
        }
    }

    for(const auto d : integer_range(2)) {
        const double d_sign = (d == 0) ? 1.0 : -1.0;

        for(const auto s : integer_range(_sections)) {
            const double s_angle = s_step * 0.5 + s * s_step;
            const double sa = s_angle + s_slip * d_sign;

            for(const auto r : integer_range(_rings + 1)) {
                const double ta = s_step * r * r_twist;
                const double vr = std::sin(sa + ta);
                const double vy = std::cos(sa + ta);
                const double r_angle = r * r_step;
                double vx = std::cos(r_angle);
                double vz = std::sin(r_angle);

                for([[maybe_unused]] const auto f : integer_range(2)) {
                    dest[k++] = float(d_sign * -vx * vr);
                    dest[k++] = float(d_sign * vy);
                    dest[k++] = float(d_sign * -vz * vr);
                }
            }
        }
    }

    assert(k <= dest.size());
}
//------------------------------------------------------------------------------
void unit_twisted_torus_gen::wrap_coords(span<float> dest) noexcept {
    assert(has(vertex_attrib_kind::wrap_coord));
    assert(dest.size() >= vertex_count() * 2);

    span_size_t k = 0;

    const double r_step = 0.5 / double(_rings);
    const double s_step = 1.0 / double(_sections);
    const double s_slip = s_step * _thickness_ratio * 2.0;

    for(const auto f : integer_range(4)) {
        const double f_sign = (f == 0) ? 1.0 : -1.0;

        for(const auto s : integer_range(_sections)) {
            const double s_angle = s_step * 0.5 + s * s_step;
            const double sa[2] = {
              s_angle + s_slip * f_sign, s_angle - s_slip * f_sign};

            for(const auto r : integer_range(_rings + 1)) {
                const double r_angle = 2.0 * r * r_step;
                const double u = r_angle;

                for(const auto d : integer_range(2)) {
                    const double v = sa[d];
                    dest[k++] = float(u);
                    dest[k++] = float(v);
                }
            }
        }
    }

    assert(k <= dest.size());
}
//------------------------------------------------------------------------------
void unit_twisted_torus_gen::attrib_values(
  const vertex_attrib_variant vav,
  span<float> dest) {
    switch(vav.attribute()) {
        case vertex_attrib_kind::position:
            positions(dest);
            break;
        case vertex_attrib_kind::normal:
            normals(dest);
            break;
        case vertex_attrib_kind::wrap_coord:
            wrap_coords(dest);
            break;
        default:
            _base::attrib_values(vav, dest);
            break;
    }
}
//------------------------------------------------------------------------------
auto unit_twisted_torus_gen::operation_count(const drawing_variant)
  -> span_size_t {
    return _sections * 4;
}
//------------------------------------------------------------------------------
void unit_twisted_torus_gen::instructions(
  const drawing_variant var,
  span<draw_operation> ops) {
    if(var == 0) {
        assert(ops.size() >= operation_count(var));

        span_size_t strip = 2 * (_rings + 1);
        span_size_t offs = 0;

        auto opi = ops.begin();
        for(const auto f : integer_range(4)) {
            for([[maybe_unused]] const auto s : integer_range(_sections)) {
                assert(opi != ops.end());
                draw_operation& op = *opi++;

                op.mode = primitive_type::triangle_strip;
                op.idx_type = index_data_type::none;
                op.first = offs;
                op.count = strip;
                op.phase = f;
                op.primitive_restart = false;
                op.cw_face_winding = true;
                offs += strip;
            }
        }
    }
}
//------------------------------------------------------------------------------
auto unit_twisted_torus_gen::bounding_sphere() -> math::sphere<float, true> {
    return {{0.F, 0.F, 0.F}, 0.5F};
}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

