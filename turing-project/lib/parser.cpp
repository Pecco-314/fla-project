#include "parser.h"
#include "error.h"
#include "lexer.h"

Parser::Parser(std::shared_ptr<Code> code, TuringMachine *tm) : code(code), tm(tm) {}

Token Parser::peek(int cnt) const {
    return it + cnt >= tokens.end() ? eofToken() : *(it + cnt);
}

Token Parser::eofToken() const {
    return tokens.back().span;
}

void Parser::next() {
    ++it;
}

void Parser::parseQ() {
    auto bg = peek();
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
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                                        code->span(brace_bg.span, peek().span)};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, peek().span};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET,
                                        code->span(brace_bg.span, peek(-1).span)};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_Q, peek().span};
    }
    tm->setSpan("Q", code->span(bg.span, peek(-1).span));
}

void Parser::parseS() {
    auto bg = peek();
    if (peek().isStr("#S")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('{')) {
                auto brace_bg = peek();
                next();
                for (;;) {
                    if (peek().isValidChar()) {
                        tm->addInputSymbol(peek().val[0]);
                        next();
                    } else if (peek().isChar('_')) {
                        throw CodeError{CodeError::Type::PARSER_UNEXPECTED_UNDERSCORE,
                                        peek().span};
                    } else if (peek().isChar('}') && brace_bg == peek(-1)) {
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                                        code->span(brace_bg.span, peek().span)};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_VALID_CHAR,
                                        peek().span};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET,
                                        code->span(brace_bg.span, peek(-1).span)};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_S, peek().span};
    }
    tm->setSpan("S", code->span(bg.span, peek(-1).span));
}

void Parser::parseG() {
    auto bg = peek();
    if (peek().isStr("#G")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('{')) {
                auto brace_bg = peek();
                next();
                for (;;) {
                    if (peek().isValidChar()) {
                        tm->addTapeSymbol(peek().val[0]);
                        next();
                    } else if (peek().isChar('}') && brace_bg == peek(-1)) {
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                                        code->span(brace_bg.span, peek().span)};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_VALID_CHAR,
                                        peek().span};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET,
                                        code->span(brace_bg.span, peek(-1).span)};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_G, peek().span};
    }
    tm->setSpan("G", code->span(bg.span, peek(-1).span));
}

void Parser::parseq0() {
    auto bg = peek();
    if (peek().isStr("#q0")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isID()) {
                tm->setInitialState(peek().val);
                next();
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_Q0, peek().span};
    }
    tm->setSpan("q0", code->span(bg.span, peek(-1).span));
}

void Parser::parseB() {
    auto bg = peek();
    if (peek().isStr("#B")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('_')) {
                next();
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_UNDERSCORE_AS_BLANK,
                                peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_B, peek().span};
    }
    tm->setSpan("B", code->span(bg.span, peek(-1).span));
}

void Parser::parseF() {
    auto bg = peek();
    if (peek().isStr("#F")) {
        next();
        if (peek().isChar('=')) {
            next();
            if (peek().isChar('{')) {
                auto brace_bg = peek();
                next();
                for (;;) {
                    if (peek().isID()) {
                        tm->addFinalState(peek().val);
                        next();
                    } else if (peek().isChar('}') && brace_bg == peek(-1)) {
                        throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                                        code->span(brace_bg.span, peek().span)};
                    } else {
                        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, peek().span};
                    }
                    if (peek().isChar('}')) {
                        next();
                        break;
                    } else if (peek().isChar(',')) {
                        next();
                        continue;
                    } else {
                        throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET,
                                        code->span(brace_bg.span, peek(-1).span)};
                    }
                }
            } else {
                throw CodeError{CodeError::Type::PARSER_EXPECTED_LBRACE, peek().span};
            }
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_EQUAL, peek().span};
        }
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_F, peek().span};
    }
    tm->setSpan("F", code->span(bg.span, peek(-1).span));
}

void Parser::parse() {
    auto lexer = std::make_shared<Lexer>(code);
    tokens = lexer->lex();
    it = tokens.begin();
    parseQ();
    parseS();
    parseG();
    parseq0();
    parseB();
    parseF();
    tm->validate();
}
