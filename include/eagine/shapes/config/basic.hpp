/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#ifndef EAGINE_SHAPES_CONFIG_BASIC_HPP
#define EAGINE_SHAPES_CONFIG_BASIC_HPP

#include <eagine/config/basic.hpp>

#ifndef EAGINE_SHAPES_LIBRARY
#if defined(__clang_analyzer__)
#define EAGINE_SHAPES_LIBRARY 1
#else
#define EAGINE_SHAPES_LIBRARY 0
#endif
#endif

#endif // EAGINE_SHAPES_CONFIG_BASIC_HPP
