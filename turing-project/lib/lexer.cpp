#include "lexer.h"
#include "error.h"
Lexer::Lexer(std::shared_ptr<Code> code)
    : code(code), st(code->begin()), ed(st), status(Status::INITIAL) {}

std::vector<Token> Lexer::lex() {
    while (status != Status::END) {
        switch (status) {
        case Status::INITIAL:
            if (chr() == '\0') {
                status = Status::END;
            } else if (isspace(chr())) {
                skip();
            } else if (chr() == ';') {
                skipLine();
            } else if (isalnum(chr()) || chr() == '_') {
                extend();
                status = Status::WORD;
            } else if (chr() == '#') {
                extend();
                status = Status::PARAM;
            } else if (isgraph(chr())) {
                extend();
                store();
            } else {
                throw CodeError{CodeError::Type::LEXER_INVALID_CHAR, st, ed};
            }
            break;
        case Status::WORD:
            if (isalnum(chr()) || chr() == '_') {
                extend();
            } else {
                store();
                status = Status::INITIAL;
            }
            break;
        case Status::PARAM:
            if (isalnum(chr()) || chr() == '_') {
                extend();
            } else {
                store();
                status = Status::INITIAL;
            }
            break;
        case Status::END:
            break;
        }
    }
    return tokens;
}

void Lexer::skip() {
    ed = ++st;
}

void Lexer::skipLine() {
    ed = st.skipLine();
}

void Lexer::extend() {
    ++ed;
}

char Lexer::chr() const {
    return *ed;
}

std::string Lexer::span() const {
    return st.span(ed);
}

void Lexer::store() {
    tokens.emplace_back(span(), st, ed);
    st = ed;
}
