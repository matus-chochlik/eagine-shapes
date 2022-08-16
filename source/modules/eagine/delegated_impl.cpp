/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
module;

#include <cassert>

module eagine.shapes;
import eagine.core.types;
import eagine.core.memory;
import eagine.core.utility;
import eagine.core.math;

namespace eagine::shapes {
//------------------------------------------------------------------------------
// delegated_generator
//------------------------------------------------------------------------------
delegated_gen::delegated_gen(std::shared_ptr<generator> gen) noexcept
  : _gen{std::move(gen)} {}
//------------------------------------------------------------------------------
} // namespace eagine::shapes

