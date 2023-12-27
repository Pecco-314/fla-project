#include "parser.h"
#include "error.h"
#include "lexer.h"

Parser::Parser(std::shared_ptr<Code> code, TuringMachine *tm) : code(code), tm(tm) {}

Token Parser::peek(int cnt) const {
    return it + cnt >= tokens.end() ? eofToken() : *(it + cnt);
}

Token Parser::eofToken() const {
    return code->span(code->end(), code->end());
}

void Parser::next() {
    ++it;
}

bool Parser::parseText(std::string_view s, bool throws) {
    if (peek().isStr(s)) {
        next();
        return true;
    } else {
        if (throws) {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_TEXT, peek().span,
                            std::string{s}};
        }
        return false;
    }
}

bool Parser::parseChar(char c, bool throws) {
    if (peek().isChar(c)) {
        next();
        return true;
    } else {
        if (throws) {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_TEXT, peek().span,
                            std::string{c}};
        }
        return false;
    }
}

void Parser::parseQ() {
    auto bg = peek();
    parseText("#Q") && parseChar('=') && parseChar('{');
    auto brace_bg = peek(-1);
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
    tm->setSpan("Q", code->span(bg.span, peek(-1).span));
}

void Parser::parseS() {
    auto bg = peek();
    parseText("#S") && parseChar('=') && parseChar('{');
    auto brace_bg = peek(-1);
    for (;;) {
        if (peek().isValidChar()) {
            tm->addInputSymbol(peek().val[0]);
            next();
        } else if (peek().isChar('_')) {
            throw CodeError{CodeError::Type::PARSER_UNEXPECTED_UNDERSCORE, peek().span};
        } else if (peek().isChar('}') && brace_bg == peek(-1)) {
            throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                            code->span(brace_bg.span, peek().span)};
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_VALID_CHAR, peek().span};
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
    tm->setSpan("S", code->span(bg.span, peek(-1).span));
}

void Parser::parseG() {
    auto bg = peek();
    parseText("#G") && parseChar('=') && parseChar('{');
    auto brace_bg = peek(-1);
    for (;;) {
        if (peek().isValidChar()) {
            tm->addTapeSymbol(peek().val[0]);
            next();
        } else if (peek().isChar('}') && brace_bg == peek(-1)) {
            throw CodeError{CodeError::Type::PARSER_EMPTY_SET,
                            code->span(brace_bg.span, peek().span)};
        } else {
            throw CodeError{CodeError::Type::PARSER_EXPECTED_VALID_CHAR, peek().span};
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
    tm->setSpan("G", code->span(bg.span, peek(-1).span));
}

void Parser::parseq0() {
    auto bg = peek();
    parseText("#q0") && parseChar('=');
    if (peek().isID()) {
        tm->setInitialState(peek().val);
        next();
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_ID, peek().span};
    }
    tm->setSpan("q0", code->span(bg.span, peek(-1).span));
}

void Parser::parseB() {
    auto bg = peek();
    parseText("#B") && parseChar('=');
    if (peek().isChar('_')) {
        next();
    } else {
        throw CodeError{CodeError::Type::PARSER_EXPECTED_UNDERSCORE_AS_BLANK,
                        peek().span};
    }
    tm->setSpan("B", code->span(bg.span, peek(-1).span));
}

void Parser::parseF() {
    auto bg = peek();
    parseText("#F") && parseChar('=') && parseChar('{');
    auto brace_bg = peek(-1);
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
    tm->setSpan("F", code->span(bg.span, peek(-1).span));
}

void Parser::parseN() {
    auto bg = peek();
    parseText("#N") && parseChar('=');
    auto intval = peek().toInt();
    tm->setNumTapes(intval);
    next();
    tm->setSpan("N", code->span(bg.span, peek(-1).span));
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
    parseN();
    tm->validate();
}
