# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
# https://www.boost.org/LICENSE_1_0.txt

add_subdirectory(to_json)
add_subdirectory(add_occlusion)
add_subdirectory(add_adjacency)
add_subdirectory(add_primitive_info)

eagine_add_license(shapes-apps)
eagine_add_debian_changelog(shapes-apps)

