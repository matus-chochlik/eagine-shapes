# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt

add_library(
	eagine-shapes
	shapes_base.cpp
	shapes_topology.cpp
	shapes_cube.cpp
	shapes_round_cube.cpp
	shapes_sphere.cpp
	shapes_torus.cpp
	shapes_twisted_torus.cpp
	shapes_icosahedron.cpp
	shapes_value_tree.cpp
	shapes_modifiers.cpp
	shapes_adjacency.cpp
	shapes_to_quads.cpp
	shapes_to_patches.cpp
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
