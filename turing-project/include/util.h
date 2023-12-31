#ifndef __UTIL_H__
#define __UTIL_H__

#include "color.h"
#include <iostream>
#include <sstream>

namespace util {

std::string quoted(char ch);

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
