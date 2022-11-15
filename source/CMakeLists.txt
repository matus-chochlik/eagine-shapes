# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
add_subdirectory(modules)
add_subdirectory(app)

set(
	EAGINE_SHAPES_EXPORT_TARGETS
	eagine-shapes-to_json
	eagine-shapes-add_occlusion
	eagine-shapes-add_adjacency
	eagine-shapes-add_primitive_info
)

if(TARGET eagine-shapes)
	add_library(EAGine::Shapes INTERFACE IMPORTED GLOBAL)
	target_link_libraries(
		EAGine::Shapes
		INTERFACE
			eagine-shapes-headers
			eagine-shapes
	)

	list(
		APPEND EAGINE_SHAPES_EXPORT_TARGETS
		eagine-shapes-private-headers
		eagine-shapes-headers
		eagine-shapes
	)

	install(
		EXPORT Shapes
		NAMESPACE EAGine::
		FILE Shapes.cmake
		DESTINATION lib/cmake/EAGine
	)
endif()

install(
	TARGETS ${EAGINE_SHAPES_EXPORT_TARGETS}
	EXPORT Shapes
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	PUBLIC_HEADER DESTINATION include
)

