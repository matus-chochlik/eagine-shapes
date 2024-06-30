# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt

add_executable(eagine-shapes-add-primitive-info main.cpp)
eagine_add_exe_analysis(eagine-shapes-add-primitive-info)
eagine_target_modules(
	eagine-shapes-add-primitive-info
	std
	eagine.core
	eagine.shapes
)

install(
	TARGETS eagine-shapes-add-primitive-info
	COMPONENT shapes-apps
	RUNTIME DESTINATION bin)

