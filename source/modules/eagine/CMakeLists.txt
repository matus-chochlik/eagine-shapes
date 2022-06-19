# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
eagine_add_module(
	eagine.shapes PARTITION generator_capabilities
	IMPORTS
		eagine.core.types
)

eagine_add_module(
	eagine.shapes PARTITION vertex_attrib
	IMPORTS
		eagine.core.types
		eagine.core.reflection
)

eagine_add_module(
	eagine.shapes PARTITION drawing
	IMPORTS
		eagine.core.types
		eagine.core.identifier
		eagine.core.reflection
)

eagine_add_module(
	eagine.shapes PARTITION generator
	IMPORTS
		generator_capabilities vertex_attrib drawing
		eagine.core.types
		eagine.core.memory
		eagine.core.container
		eagine.core.reflection
		eagine.core.utility
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION screen
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION plane
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.valid_if
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION cube
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION round_cube
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION skybox
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION icosahedron
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION sphere
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.valid_if
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION torus
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.valid_if
		eagine.core.utility
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION twisted_torus
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.valid_if
		eagine.core.utility
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION value_tree
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.reflection
		eagine.core.valid_if
		eagine.core.utility
		eagine.core.math
		eagine.core.value_tree
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION delegated
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION combined
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.utility
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION cached
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
		eagine.core.logging
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION array
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION centered
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION occluded
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.units
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION reboxed
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
)

eagine_add_module(
	eagine.shapes
	PP_NAME SHAPES
	INTERFACES shapes
	PARTITIONS
		generator_capabilities
		vertex_attrib
		drawing
		generator
		screen
		plane
		cube
		round_cube
		skybox
		icosahedron
		sphere
		torus
		twisted_torus
		value_tree
		delegated
		combined
		cached
		array
		centered
		occluded
		reboxed
	SOURCES
		generator_impl
		screen_impl
		plane_impl
		cube_impl
		round_cube_impl
		skybox_impl
		icosahedron_impl
		sphere_impl
		torus_impl
		twisted_torus_impl
		value_tree_impl
		combined_impl
		cached_impl
		array_impl
		centered_impl
		occluded_impl
		reboxed_impl
)

