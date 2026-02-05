#pragma once

#include <cstdio>
#include <string_view>

namespace term {

// clang-format off
const char ansi_escape= '\x1b';
/// Lower bound for the last escape sequence byte
const char ansi_escape_ends_lower = '@' ;
/// Upper bound for the last escape sequence byte
const char ansi_escape_ends_upper =  '~';
constexpr std::string_view reset = "\x1b[m";
constexpr std::string_view hide_cursor = "\x1b[?25l";
constexpr std::string_view show_cursor = "\x1b[?25h";
constexpr std::string_view save_pos = "\x1b" "7";
constexpr std::string_view restore_pos = "\x1b" "8";
constexpr std::string_view RESET_BACKGROUND = "\x1b[49m";
constexpr std::string_view RESET_FOREGROUND = "\x1b[39m";
// clang-format on

bool is_tty(int fd);
bool is_tty(FILE *stream);
bool is_truecolor();

} // namespace term
