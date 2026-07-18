/*
 * Copyright 2025 Love Billenius

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 *  1. Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.

 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.

 *  3. Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "terminal.hpp"
#include <cstdlib>
#include <cstring>

#ifdef _WIN32

#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define fileno _fileno
#define isatty _isatty

#else

#include <unistd.h>

#endif

namespace term {

bool is_tty(const int fd) { return isatty(fd) != 0; }
bool is_tty(FILE *stream) { return is_tty(fileno(stream)); }

bool is_truecolor() {
    const char *colorterm = std::getenv("COLORTERM");
    return colorterm && (std::strstr(colorterm, "truecolor") || std::strstr(colorterm, "24bit"));
}

#ifdef _WIN32
namespace {
UINT previous_codepage = 0;

// The output codepage outlives the process (like chcp), so put it back
void restore_codepage() {
    if (previous_codepage != 0)
        SetConsoleOutputCP(previous_codepage);
}
} // namespace

void setup_console() {
    const UINT current = GetConsoleOutputCP();
    if (current != 0 && current != CP_UTF8) {
        previous_codepage = current;
        std::atexit(restore_codepage);
    }
    SetConsoleOutputCP(CP_UTF8);

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (out != INVALID_HANDLE_VALUE && GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void setup_console() {}
#endif


} // namespace term
