#ifndef __UTIL_H__
#define __UTIL_H__

#include "color.h"
#include <filesystem>
#include <iostream>
#include <set>
#include <sstream>
#ifndef NDEBUG
#include <cassert>
#endif

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

inline std::string quoted(std::string_view s) {
    std::string quoted = "\"";
    for (auto &&ch : s) {
        quoted += escaped(ch);
    }
    quoted += "\"";
    return quoted;
}

inline std::string banner(std::string_view info = "", char ch = '=', int width = 45) {
    int d = width - (int)info.size();
    if (!info.empty()) { d -= 2; }
    std::string banner = std::string(d / 2, ch);
    if (!info.empty()) { banner += " " + std::string(info) + " "; }
    banner += std::string(d - d / 2, ch);
    return Painted(banner, BOLD).str();
}

template <typename T> inline std::string to_string(const T &val) {
    std::stringstream ss;
    ss << val;
    return ss.str();
}

template <typename T, typename U>
void simpletest(std::string_view name, const T &actual, const U &expected) {
    if (actual != expected) {
        std::cerr << FAILED << name << std::endl;
        auto expected_str = to_string(expected);
        auto actual_str = to_string(actual);
        if (expected_str.find('\n') != std::string::npos ||
            actual_str.find('\n') != std::string::npos) {
            std::cerr << "expected: " << std::endl << expected_str << std::endl;
            std::cerr << "actual: " << std::endl << actual_str << std::endl;
        } else {
            std::cerr << "expected: " << expected_str << std::endl;
            std::cerr << "  actual: " << actual_str << std::endl;
        }
        exit(1);
    } else {
        std::cerr << PASSED << name << std::endl;
    }
}

inline std::string strip(std::string_view s) {
    int l = 0, r = (int)s.size() - 1;
    while (l <= r && isspace(s[l])) {
        l++;
    }
    while (l <= r && isspace(s[r])) {
        r--;
    }
    return std::string(s.substr(l, r - l + 1));
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

template <typename T> std::ostream &operator<<(std::ostream &os, const std::set<T> &s) {
    os << "{";
    for (auto it = s.begin(); it != s.end(); it++) {
        if constexpr (std::is_same_v<T, char> ||
                      std::is_convertible_v<T, std::string_view>) {
            os << quoted(*it);
        } else {
            os << *it;
        }
        if (it != --s.end()) { os << ", "; }
    }
    os << "}";
    return os;
}

inline std::filesystem::path findFileWithPrefix(std::string_view prefix) {
    std::filesystem::path cur = std::filesystem::current_path();
    for (auto &&p : std::filesystem::directory_iterator(cur)) {
        if (p.is_regular_file() && p.path().filename().string().find(prefix) == 0) {
            return p.path();
        }
    }
    std::cerr << ERR << "turing executable file not found" << std::endl;
    exit(1);
}

inline int runCommand(std::filesystem::path cmd, std::filesystem::path out,
                      std::filesystem::path err,
                      const std::vector<std::string> &args) {
    std::stringstream ss;
    ss << cmd << " ";
    for (auto &&arg : args) {
        ss << quoted(arg) << " ";
    }
    if (!out.empty()) { ss << " >" << out; }
    if (!err.empty()) { ss << " 2>" << err; }
    return WEXITSTATUS(system(ss.str().c_str()));
}

inline void assume(bool expr) {
#ifndef NDEBUG
    assert(expr);
#else
#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
    if (!expr) { __builtin_unreachable(); }
#elif defined(_MSC_VER)
    if (!expr) { __assume(0); }
#endif
#endif
}

#define TEST_ERROR(name, codes, error_type, error_check)                                 \
    try {                                                                                \
        codes;                                                                           \
        std::cerr << FAILED << name << std::endl;                                        \
        std::cerr << "should throw " << #error_type << std::endl;                        \
        exit(1);                                                                         \
    } catch (const error_type &e) { error_check }

} // namespace util

#endif
