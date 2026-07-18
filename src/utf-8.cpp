#include "utf-8.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace utf8 {

size_t get_sequence_length(const unsigned char c) {
    if ((c & 0x80) == 0) {
        return 1;
    } else if ((c & 0xE0) == 0xC0) {
        return 2;
    } else if ((c & 0xF0) == 0xE0) {
        return 3;
    } else if ((c & 0xF8) == 0xF0) {
        return 4;
    }
    return 1;
}

bool is_valid(std::string_view text) {
    size_t i = 0;
    while (i < text.size()) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        if ((c & 0x80) == 0) {
            i++;
            continue;
        }

        size_t len = 0;
        if ((c & 0xE0) == 0xC0)
            len = 2;
        else if ((c & 0xF0) == 0xE0)
            len = 3;
        else if ((c & 0xF8) == 0xF0)
            len = 4;
        else
            return false; // Invalid start byte

        if (i + len > text.size())
            return true;

        // continuation bytes
        for (size_t j = 1; j < len; ++j) {
            if ((static_cast<unsigned char>(text[i + j]) & 0xC0) != 0x80)
                return false;
        }

        if (len == 2 && c < 0xC2)
            return false;

        i += len;
    }
    return true;
}

#ifdef _WIN32
void ensure_utf8(std::string &text) {
    if (text.empty() || is_valid(text))
        return;

    UINT codepage = GetConsoleCP();
    if (codepage == 0 || codepage == CP_UTF8)
        codepage = GetACP();

    const int wide_len = MultiByteToWideChar(codepage, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
    if (wide_len <= 0)
        return;

    std::wstring wide(static_cast<size_t>(wide_len), L'\0');
    MultiByteToWideChar(codepage, 0, text.data(), static_cast<int>(text.size()), wide.data(), wide_len);

    const int utf8_len = WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_len, nullptr, 0, nullptr, nullptr);
    if (utf8_len <= 0)
        return;

    std::string converted(static_cast<size_t>(utf8_len), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_len, converted.data(), utf8_len, nullptr, nullptr);
    text = std::move(converted);
}
#else
void ensure_utf8(std::string &) {}
#endif

} // namespace utf8
