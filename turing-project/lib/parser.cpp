#include "parser.h"
#include "error.h"
#include "lexer.h"

Parser::Parser(std::shared_ptr<Code> code, TuringMachine *tm)
    : code(code), tm(tm) {}

Token Parser::peek(int cnt) const {
    return it + cnt >= tokens.end() ? Token{} : *(it + cnt);
}

void Parser::parseQ() {
    if (peek().isStr("#Q")) {
        ++it;
        if (peek().isChar('=')) {
            ++it;
            if (peek().isChar('{')) {
                auto brace_bg = it;
                ++it;
                for (;;) {
                    if (peek().isID()) {
                        tm->addState(peek().val);
                        ++it;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, code, it->st,
                                        it->ed};
                    }
                    if (peek().isChar('}')) {
                        ++it;
                        break;
                    } else if (peek().isChar(',')) {
                        ++it;
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_BRACE, code,
                                        brace_bg->st, it->st};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, code, it->st,
                                it->ed};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, code, it->st, it->ed};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_Q, code, it->st, it->ed};
    }
}

void Parser::parse() {
    auto lexer = std::make_shared<Lexer>(code);
    tokens = lexer->lex();
    it = tokens.begin();
    parseQ();
}
