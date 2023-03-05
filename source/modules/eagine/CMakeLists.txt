# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
eagine_add_module(
	eagine.shapes PARTITION vertex_attributes
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
		vertex_attributes drawing
		eagine.core.types
		eagine.core.memory
		eagine.core.container
		eagine.core.reflection
		eagine.core.utility
		eagine.core.math
		eagine.core.runtime
		eagine.core.main_ctx
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
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION cube
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION round_cube
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.valid_if
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION skybox
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION icosahedron
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION tetrahedrons
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
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
		eagine.core.runtime
		eagine.core.main_ctx
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
		eagine.core.runtime
		eagine.core.main_ctx
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
		eagine.core.runtime
		eagine.core.main_ctx
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
	eagine.shapes PARTITION from_json
	IMPORTS
		generator value_tree
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.runtime
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
		eagine.core.utility
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
	eagine.shapes PARTITION primitive_info
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.units
		eagine.core.math
		eagine.core.runtime
		eagine.core.main_ctx
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
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION reboxed
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION scaled
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION scaled_wrap_coords
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION translated
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.math
)

eagine_add_module(
	eagine.shapes PARTITION to_patches
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
)

eagine_add_module(
	eagine.shapes PARTITION to_quads
	IMPORTS
		delegated
		eagine.core.types
		eagine.core.memory
)

eagine_add_module(
	eagine.shapes PARTITION topology
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.container
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION adjacency
	IMPORTS
		delegated topology
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION surface_points
	IMPORTS
		delegated topology
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.units
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes PARTITION to_json
	IMPORTS
		generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes
	PP_NAME SHAPES
	INTERFACES shapes
	PARTITIONS
		vertex_attributes
		drawing
		generator
		screen
		plane
		cube
		round_cube
		skybox
		icosahedron
		tetrahedrons
		sphere
		torus
		twisted_torus
		value_tree
		from_json
		delegated
		combined
		cached
		array
		centered
		primitive_info
		occluded
		reboxed
		scaled
		scaled_wrap_coords
		translated
		to_patches
		to_quads
		topology
		adjacency
		surface_points
		to_json
	SOURCES
		generator
		screen
		plane
		cube
		round_cube
		skybox
		icosahedron
		tetrahedrons
		sphere
		torus
		twisted_torus
		value_tree
		from_json
		combined
		cached
		array
		centered
		primitive_info
		occluded
		reboxed
		scaled
		scaled_wrap_coords
		translated
		to_patches
		to_quads
		topology
		adjacency
		surface_points
		to_json
		shapes
)

eagine_add_module_tests(
	eagine.shapes
	UNITS
		generator_capabilities
		vertex_attributes
		screen
		plane
	IMPORTS
		eagine.core
)
