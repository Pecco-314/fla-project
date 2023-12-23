#include "parser.h"
#include "error.h"
#include "lexer.h"

Parser::Parser(std::shared_ptr<Code> code, TuringMachine *tm)
    : code(code), tm(tm) {}

Token Parser::peek(int cnt) const {
    return it + cnt >= tokens.end() ? eof() : *(it + cnt);
}

Token Parser::eof() const {
    return {"", tokens.back().ed, tokens.back().ed};
}

Token Parser::cur() {
    return it == tokens.end() ? eof() : *it;
}

void Parser::next() {
    ++it;
}

void Parser::parseQ() {
    if (peek().isStr("#Q")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('{')) {
                auto brace_bg = cur();
                next();
                for (;;) {
                    if (peek().isID()) {
                        tm->addState(peek().val);
                        next();
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, code, cur().st,
                                        cur().ed};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_BRACE, code,
                                        brace_bg.st, cur().st};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, code, cur().st,
                                cur().ed};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, code, cur().st, cur().ed};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_Q, code, cur().st, cur().ed};
    }
}

void Parser::parse() {
    auto lexer = std::make_shared<Lexer>(code);
    tokens = lexer->lex();
    it = tokens.begin();
    parseQ();
}
