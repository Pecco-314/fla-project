#include "token.h"

Token::Token(Code::Span span) : val(span.str()), span(span) {}

bool Token::operator==(const Token &other) const {
    return span == other.span;
}

bool Token::isStr(std::string_view val) const {
    return this->val == val;
}

bool Token::isChar(char c) const {
    return val.size() == 1 && val[0] == c;
}

bool Token::isID() const {
    for (auto c : val) {
        if (!isalnum(c) && c != '_') { return false; }
    }
    return true;
}

bool Token::isValidChar() const {
    return val.size() == 1 && isgraph(val[0]) && val[0] != ',' && val[0] != ';' &&
           val[0] != '{' && val[0] != '}' && val[0] != '*';
}

bool Token::isInt() const {
    for (auto c : val) {
        if (!isdigit(c)) { return false; }
    }
    return true;
}

std::string Token::strval() const {
    return val;
}

char Token::charval() const {
    return val[0];
}

int Token::intval() const {
    int ret = 0;
    for (auto c : val) {
        ret = ret * 10 + c - '0';
    }
    return ret;
}
