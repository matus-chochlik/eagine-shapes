# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt

add_executable(eagine-shapes-add_adjacency main.cpp)
eagine_add_exe_analysis(eagine-shapes-add_adjacency)
if(${WITH_MODULES})
	eagine_target_modules(
		eagine-shapes-add_adjacency
		eagine.core
		eagine.shapes
	)
else()
	target_link_libraries(
		eagine-shapes-add_adjacency
		PUBLIC EAGine::Shapes
	)
endif()

