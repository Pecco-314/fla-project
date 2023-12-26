#include "token.h"
#include "error.h"

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
    return isgraph(val[0]) && val[0] != ',' && val[0] != ';' && val[0] != '{' &&
           val[0] != '}' && val[0] != '*';
}

int Token::toInt() const {
    try {
        return std::stoi(val);
    } catch (std::invalid_argument &e) {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_INTEGER, span};
    }
}
