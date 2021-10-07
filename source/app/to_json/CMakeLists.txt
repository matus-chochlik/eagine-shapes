# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt

add_executable(eagine-shapes-to_json main.cpp)
eagine_add_exe_analysis(eagine-shapes-to_json)
target_link_libraries(
	eagine-shapes-to_json
	PUBLIC EAGine::Shapes
)

