#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "code.h"

struct Token {
    std::string val;
    Code::Cursor st;
    Code::Cursor ed;

    Token(std::string_view val, const Code::Cursor &st, const Code::Cursor &ed);
    Token() = default;
    bool isNull() const;
    bool isStr(std::string_view val) const;
    bool isChar(char c) const;
    bool isID() const;
};

#endif
