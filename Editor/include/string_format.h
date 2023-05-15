#pragma once

// Until I figure out why Clang 15 isn't working with std::format, I'll use fmt::format

#include <fmt/format.h>
#define string_format(...) fmt::format(__VA_ARGS__)
