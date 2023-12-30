#include "lexer.h"
#include "error.h"
Lexer::Lexer(std::shared_ptr<Code> code)
    : span(code, code->begin(), code->begin()), status(Status::INITIAL) {}

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
                throw CodeError{CodeError::Type::LEXER_INVALID_CHAR, span};
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
    span.extend();
    span.collapse();
}

void Lexer::skipLine() {
    span.extendLine();
    span.collapse();
}

void Lexer::extend() {
    span.extend();
}

char Lexer::chr() const {
    return *span.end();
}

std::string Lexer::spanStr() const {
    return span.str();
}

void Lexer::store() {
    tokens.push_back(span);
    span.collapse();
}
