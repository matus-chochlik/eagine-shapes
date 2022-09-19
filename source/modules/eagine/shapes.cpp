/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
export module eagine.shapes;

import eagine.core.runtime;
import eagine.core.main_ctx;

export import :vertex_attrib;
export import :generator_capabilities;
export import :drawing;
export import :generator;
export import :delegated;
export import :screen;
export import :plane;
export import :cube;
export import :round_cube;
export import :skybox;
export import :icosahedron;
export import :tetrahedrons;
export import :sphere;
export import :torus;
export import :twisted_torus;
export import :value_tree;
export import :combined;
export import :cached;
export import :array;
export import :centered;
export import :occluded;
export import :reboxed;
export import :scaled;
export import :scaled_wrap_coords;
export import :translated;
export import :to_patches;
export import :to_quads;
export import :topology;
export import :adjacency;
export import :surface_points;
export import :from_json;
export import :to_json;

namespace eagine::shapes {
//------------------------------------------------------------------------------
export auto shape_from(const url&, main_ctx&) -> std::unique_ptr<generator>;
//------------------------------------------------------------------------------
} // namespace eagine::shapes
