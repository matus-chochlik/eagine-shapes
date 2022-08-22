# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
if(${WITH_MODULES})
	add_subdirectory(modules)
else()
	add_subdirectory(lib)
endif()
add_subdirectory(app)

if(TARGET eagine-shapes)
	add_library(EAGine::Shapes INTERFACE IMPORTED GLOBAL)
	target_link_libraries(EAGine::Shapes INTERFACE eagine-shapes-headers)

	target_link_libraries(
		EAGine::Shapes
		INTERFACE
			eagine-shapes
	)

	set(
		EAGINE_SHAPES_EXPORT_TARGETS
		eagine-shapes-private-headers
		eagine-shapes-headers
		eagine-shapes
		eagine-shapes-to_json
		eagine-shapes-add_adjacency
		eagine-shapes-add_occlusion
	)

	install(
		TARGETS ${EAGINE_SHAPES_EXPORT_TARGETS}
		EXPORT Shapes
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
		PUBLIC_HEADER DESTINATION include
	)

	install(
		EXPORT Shapes
		NAMESPACE EAGine::
		FILE Shapes.cmake
		DESTINATION lib/cmake/EAGine
	)
endif()
