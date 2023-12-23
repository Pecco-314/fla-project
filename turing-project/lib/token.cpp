#include "token.h"

Token::Token(std::string_view val, const Code::Cursor &st, const Code::Cursor &ed)
    : val(val), st(st), ed(ed) {}

bool Token::operator==(const Token &other) const {
    return st == other.st && ed == other.ed;
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
    return isgraph(val[0]) && val[0] != ',' && val[0] != ';' && val[0] != '{' &&
           val[0] != '}' && val[0] != '*' && val[0] != '_';
}
