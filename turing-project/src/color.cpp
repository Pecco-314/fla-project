#include "color.h"
#include "util.h"

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
    if (colors.empty()) { return ret; }
    ret += "\033[";
    for (auto it = colors.begin(); it != colors.end(); ++it) {
        ret += std::to_string(*it);
        ret += it + 1 == colors.end() ? "m" : ";";
    }
    return ret;
}

std::ostream &operator<<(std::ostream &os, const TermColor &color) {
    if (TermColor::force_color || util::isatty(os)) {
        return os << color.str();
    } else {
        return os;
    }
}

std::string Painted::str() const {
    return color.str() + text + RESET.str();
}

std::string Painted::plaintext() const {
    return text;
}

std::ostream &operator<<(std::ostream &os, const Painted &painted) {
    return os << painted.color << painted.text << RESET;
}
