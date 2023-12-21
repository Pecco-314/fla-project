#include "token.h"

Token::Token(std::string_view val, const Code::Cursor &st, const Code::Cursor &ed)
    : val(val), st(st), ed(ed) {}
