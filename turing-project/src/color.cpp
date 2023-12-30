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

std::ostream &operator<<(std::ostream &os, const TermColor &color) {
    if (TermColor::force_color || isatty(STDERR_FILENO)) {
        for (auto c : color.colors) {
            os << "\033[" << c << "m";
        }
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Painted &paint) {
    os << paint.color << paint.text << RESET;
    return os;
}
