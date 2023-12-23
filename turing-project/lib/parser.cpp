#include "parser.h"
#include "error.h"
#include "lexer.h"

Parser::Parser(std::shared_ptr<Code> code, TuringMachine *tm) : code(code), tm(tm) {}

Token Parser::peek(int cnt) const {
    return it + cnt >= tokens.end() ? eof() : *(it + cnt);
}

Token Parser::eof() const {
    return {"", tokens.back().ed, tokens.back().ed};
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
                auto brace_bg = peek();
                next();
                for (;;) {
                    if (peek().isID()) {
                        tm->addState(peek().val);
                        next();
                    } else if (peek().isChar('}') && brace_bg == peek(-1)) {
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET, code,
                                        brace_bg.st, peek().ed};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, code,
                                        peek().st, peek().ed};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET, code,
                                        brace_bg.st, peek().st};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, code, peek().st,
                                peek().ed};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, code, peek().st,
                            peek().ed};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_Q, code, peek().st, peek().ed};
    }
}

void Parser::parseS() {
    if (peek().isStr("#S")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('{')) {
                auto brace_bg = peek();
                next();
                for (;;) {
                    if (peek().isValidChar()) {
                        tm->addSymbol(peek().val[0]);
                        next();
                    } else if (peek().isChar('}') && brace_bg == peek(-1)) {
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET, code,
                                        brace_bg.st, peek().ed};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_VALID_CHAR, code,
                                        peek().st, peek().ed};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET, code,
                                        brace_bg.st, peek().st};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, code, peek().st,
                                peek().ed};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, code, peek().st,
                            peek().ed};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_S, code, peek().st, peek().ed};
    }
}

void Parser::parse() {
    auto lexer = std::make_shared<Lexer>(code);
    tokens = lexer->lex();
    it = tokens.begin();
    parseQ();
    parseS();
}
