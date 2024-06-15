#  Copyright Matus Chochlik.
#  Distributed under the Boost Software License, Version 1.0.
#  See accompanying file LICENSE_1_0.txt or copy at
#  https://www.boost.org/LICENSE_1_0.txt
#
# Package specific options
#  Debian
#   Dependencies
set(CPACK_DEBIAN_SHAPES-APPS_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_SHAPES-DEV_PACKAGE_DEPENDS "eagine-core-dev (>= @EAGINE_VERSION@)")
#   Descriptions
set(CPACK_DEBIAN_-SHAPES-APPS_DESCRIPTION "Collection of EAGine geometry shape generator applications.")
set(CPACK_DEBIAN_-SHAPES-DEV_DESCRIPTION "C++ generator of geometric shapes used for rendering.")

