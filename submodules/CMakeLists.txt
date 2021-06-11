# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
#

if(NOT TARGET EAGine::Core)
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/eagine-core/CMakeLists.txt")
		execute_process(
			COMMAND git submodule update --init
			WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/eagine-core"
		)
	endif()
	add_subdirectory(eagine-core EXCLUDE_FROM_ALL)
endif()
