#pragma once

#ifdef USE_LIBFMT
#include <fmt/format.h>

namespace ff {

using fmt::format;
using fmt::make_format_args;
using fmt::vformat;

} // namespace ff

#else
#include <format>

namespace ff {

using std::format;
using std::make_format_args;
using std::vformat;

} // namespace ff

#endif
