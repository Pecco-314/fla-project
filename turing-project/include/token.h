#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "code.h"

struct Token {
    std::string val;
    Code::Span span;

    Token(Code::Span span);
    Token() = default;
    bool operator==(const Token &other) const;
    bool isStr(std::string_view val) const;
    bool isChar(char c) const;
    bool isID() const;
    bool isValidChar() const;
    bool isPositiveInt() const;
    std::string strval() const;
    char charval() const;
    int intval() const;
    void operator+=(const Token &other);
};

#endif
