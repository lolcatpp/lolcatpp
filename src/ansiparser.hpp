#pragma once

#include <iterator>
#include <string_view>

struct AnsiToken {
    std::string_view content;
    bool is_escape;
};

class AnsiParser {
public:
    explicit AnsiParser(std::string_view line) : m_data(line) {}

    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = AnsiToken;
        using difference_type = std::ptrdiff_t;
        using pointer = AnsiToken *;
        using reference = AnsiToken &;

        Iterator(std::string_view d, size_t p) : m_data(d), m_posistion(p) {}
        Iterator &operator++();
        Iterator operator++(int);
        AnsiToken operator*() const;
        bool operator!=(const Iterator &other) const;

    private:
        std::string_view m_data;
        size_t m_posistion = 0;
        AnsiToken m_current{};
    };

    Iterator begin();
    Iterator end();

private:
    std::string_view m_data;
};
