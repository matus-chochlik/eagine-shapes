# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt
#
cmake_minimum_required(VERSION 3.16)
project(eagine_shapes_deb_test CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package(EAGine REQUIRED COMPONENTS Shapes)

if(EAGineShapes_FOUND)
	if(TARGET EAGine::Shapes)
		message("EAGine Shapes found")
		add_subdirectory(example)
	else()
		message(FATAL_ERROR "EAGine Shapes target not found")
	endif()
else()
	message(FATAL_ERROR "EAGine Shapes package not found")
endif()

