# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt

add_library(
	eagine-shapes
	base.cpp
	topology.cpp
	cube.cpp
	round_cube.cpp
	sphere.cpp
	torus.cpp
	twisted_torus.cpp
	icosahedron.cpp
	tetrahedrons.cpp
	value_tree.cpp
	modifiers.cpp
	adjacency.cpp
	surface_points.cpp
	to_quads.cpp
	to_patches.cpp
	to_json.cpp
	from_json.cpp
)
eagine_add_lib_analysis(eagine-shapes)

target_compile_definitions(
	eagine-shapes PUBLIC EAGINE_SHAPES_LIBRARY=1
)

target_include_directories(
	eagine-shapes
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_link_libraries(
	eagine-shapes
	PUBLIC
		EAGine::Core
		eagine-shapes-headers
	PRIVATE
		eagine-shapes-private-headers
)
