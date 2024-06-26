# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt
eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION vertex_attributes
	IMPORTS
		std
		eagine.core.types
		eagine.core.memory
		eagine.core.math
		eagine.core.container
		eagine.core.reflection
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION drawing
	IMPORTS
		std
		eagine.core.types
		eagine.core.identifier
		eagine.core.reflection
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION generator
	IMPORTS
		std vertex_attributes drawing
		eagine.core.types
		eagine.core.memory
		eagine.core.container
		eagine.core.reflection
		eagine.core.utility
		eagine.core.math
		eagine.core.runtime
		eagine.core.valid_if
		eagine.core.value_tree
		eagine.core.main_ctx
		eagine.core.resource
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION delegated
	IMPORTS
		std generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.utility
		eagine.core.valid_if
		eagine.core.math
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION primitive_info
	IMPORTS
		std delegated
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.units
		eagine.core.math
		eagine.core.runtime
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION topology
	IMPORTS
		std generator
		eagine.core.types
		eagine.core.memory
		eagine.core.identifier
		eagine.core.container
		eagine.core.math
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	PARTITION to_json
	IMPORTS
		std generator
		eagine.core.types
		eagine.core.memory
		eagine.core.runtime
		eagine.core.math
		eagine.core.identifier
		eagine.core.main_ctx
)

eagine_add_module(
	eagine.shapes
	COMPONENT shapes-dev
	SOURCES
		vertex_attributes
		generator
		drawing
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
	IMPORTS
		std
		eagine.core
)

eagine_add_module_tests(
	eagine.shapes
	UNITS
		generator_capabilities
		vertex_attributes
		screen
		plane
		models
	IMPORTS
		std
		eagine.core
		eagine.core.main_ctx
)

eagine_embed_target_resources(
	TARGET eagine.shapes
	RESOURCES
		Cube       "resources/cube.json"
		SpikHedron "resources/spikosahedron.json"
	PACKED)

eagine_add_license(shapes-dev)
eagine_add_debian_changelog(shapes-dev)

eagine_add_package_property(
	shapes-dev
	GENERATOR DEBIAN
	VARIABLE PACKAGE_DEPENDS
	VALUE "eagine-core-dev (>= ${EAGINE_VERSION})")

eagine_add_package_property(
	shapes-dev
	GENERATOR DEBIAN
	VARIABLE DESCRIPTION
	VALUE "C++ generators of geometric shapes used for rendering.")

