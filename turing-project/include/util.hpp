#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include "color.h"
#include <iostream>
#include <sstream>

namespace util {

inline std::string escaped(char ch) {
    if (ch == '\n') {
        return "\\n";
    } else if (ch == '\t') {
        return "\\t";
    } else if (ch == '\r') {
        return "\\r";
    } else if (ch == '\0') {
        return "\\0";
    } else if (ch == '\'') {
        return "\\'";
    } else if (ch == '\\') {
        return "\\\\";
    } else if (ch == '\b') {
        return "\\b";
    } else if (ch == '\f') {
        return "\\f";
    } else if (ch == '\v') {
        return "\\v";
    } else if (ch == '\a') {
        return "\\a";
    } else if (isprint(ch)) {
        return {ch};
    } else {
        static const char HEX[] = "0123456789ABCDEF";
        unsigned char c = ch;
        return {'\\', 'x', HEX[c / 16], HEX[c % 16]};
    }
}

inline std::string quoted(char ch) {
    return "'" + escaped(ch) + "'";
}

template <typename T, typename U>
void simpletest(std::string_view name, const T &actual, const U &expected) {
    if (actual != expected) {
        std::cerr << FAILED << name << std::endl;
        std::cerr << "expected: " << expected << std::endl;
        std::cerr << "  actual: " << actual << std::endl;
        exit(1);
    } else {
        std::cerr << PASSED << name << std::endl;
    }
}

inline std::string format(std::string_view fmt) {
    std::string s;
    for (int i = 0; i < (int)fmt.size(); i++) {
        if (fmt[i] == '{') {
            if (fmt[i + 1] == '{') {
                s += fmt[i];
                i++;
            } else {
                throw std::runtime_error("format error");
            }
        } else if (fmt[i] == '}') {
            if (fmt[i + 1] == '}') {
                s += fmt[i];
                i++;
            } else {
                throw std::runtime_error("format error");
            }
        } else {
            s += fmt[i];
        } 
    }
    return s;
}

template <typename... Args, typename T>
std::string format(std::string_view fmt, const T &t, Args... args) {
    std::stringstream ss;
    for (int i = 0; i < (int)fmt.size(); i++) {
        if (fmt[i] == '{') {
            if (fmt[i + 1] == '{') {
                ss << fmt[i];
                i++;
            } else if (fmt[i + 1] == '}') {
                ss << t << format(fmt.substr(i + 2), args...);
                break;
            } else {
                throw std::runtime_error("format error");
            }
        } else if (fmt[i] == '}') {
            if (fmt[i + 1] == '}') {
                ss << fmt[i];
                i++;
            } else {
                throw std::runtime_error("format error");
            }
        } else {
            ss << fmt[i];
        }
    }
    return ss.str();
}

} // namespace util

#endif
