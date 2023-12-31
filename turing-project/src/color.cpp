#include "color.h"
#include <unistd.h>

bool TermColor::force_color = false;

void TermColor::setForceColor(bool force_color) {
    TermColor::force_color = force_color;
}

TermColor TermColor::operator|(const TermColor &other) const {
    TermColor ret = *this;
    for (auto color : other.colors) {
        ret.colors.push_back(color);
    }
    return ret;
}

std::string TermColor::str() const {
    std::string ret;
    if (TermColor::force_color || isatty(STDERR_FILENO)) {
        for (auto c : colors) {
            ret += "\033[" + std::to_string(c) + "m";
        }
    }
    return ret;
}

std::ostream &operator<<(std::ostream &os, const TermColor &color) {
    return os << color.str();
}

std::string Painted::str() const {
    return color.str() + std::string(text) + RESET.str();
}

std::string operator+(std::string_view str, const Painted &paint) {
    return std::string(str) + paint.str();
}

std::string operator+(const Painted &paint, std::string_view str) {
    return paint.str() + std::string(str);
}

std::string operator+(const Painted &paint1, const Painted &paint2) {
    return paint1.str() + paint2.str();
}

std::ostream &operator<<(std::ostream &os, const Painted &painted) {
    return os << painted.str();
}
