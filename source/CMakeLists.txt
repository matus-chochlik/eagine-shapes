# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
add_subdirectory(lib)
add_subdirectory(app)

add_library(EAGine::Shapes INTERFACE IMPORTED GLOBAL)
target_link_libraries(EAGine::Shapes INTERFACE eagine-shapes-headers)

target_link_libraries(
	EAGine::Shapes
	INTERFACE
		eagine-shapes
)

get_target_property(EAGINE_CORE_LIBS EAGine::Core INTERFACE_LINK_LIBRARIES)

set(
	EAGINE_SHAPES_EXPORT_TARGETS
	eagine-shapes-private-headers
	eagine-shapes-headers
	eagine-shapes
	eagine-shapes-to_json
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
