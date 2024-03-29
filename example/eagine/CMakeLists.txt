# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt

add_custom_target(eagine-shapes-examples ALL)
set_target_properties(
	eagine-shapes-examples
	PROPERTIES FOLDER "Example/Shapes"
)

function(eagine_install_example TARGET_NAME)
	install(
		TARGETS ${TARGET_NAME}
		DESTINATION share/eagine/example/shapes
	)
endfunction()

function(eagine_example_common EXAMPLE_NAME)
	add_executable(
		eagine-${EXAMPLE_NAME}
		EXCLUDE_FROM_ALL
		"${EXAMPLE_NAME}.cpp"
		${ARGN}
	)
	add_dependencies(eagine-shapes-examples eagine-${EXAMPLE_NAME})
	eagine_add_exe_analysis(eagine-${EXAMPLE_NAME})
	eagine_target_modules(
		eagine-${EXAMPLE_NAME}
		eagine.core
		eagine.shapes
	)

	set_target_properties(
		eagine-${EXAMPLE_NAME}
		PROPERTIES FOLDER "Example/Shapes"
	)
	eagine_install_example(eagine-${EXAMPLE_NAME})
endfunction()

eagine_example_common(for_each_triangle)
eagine_example_common(shape_topology)
eagine_example_common(shape_from_url)

