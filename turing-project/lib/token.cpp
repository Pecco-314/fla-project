#include "token.h"

Token::Token(std::string_view val, const Code::Cursor &st, const Code::Cursor &ed)
    : val(val), st(st), ed(ed) {}

bool Token::isStr(std::string_view val) const {
    return this->val == val;
}

bool Token::isChar(char c) const {
    return val.size() == 1 && val[0] == c;
}

bool Token::isID() const {
    for (auto c : val) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}
