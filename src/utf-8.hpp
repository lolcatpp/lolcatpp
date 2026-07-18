#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace utf8 {

/**
 * Get the length (1-4) of a UTF-8 sequence based on the first byte
 */
[[nodiscard]] size_t get_sequence_length(unsigned char first_byte);

/**
 * Check if the (sub)string is valid UTF-8
 */
[[nodiscard]] bool is_valid(std::string_view text);

/**
 * Make sure the text is UTF-8.
 * On Windows, text that isn't valid UTF-8 is re-encoded in place from the
 * console/ANSI codepage (e.g. GBK output piped from cmd.exe). No-op elsewhere.
 */
void ensure_utf8(std::string &text);

} // namespace utf8
