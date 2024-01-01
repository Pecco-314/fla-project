#include "parser.h"
#include "error.h"
#include "lexer.h"
#include "util.hpp"

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

Token Parser::consume() {
    return it >= tokens.end() ? eofToken() : *it++;
}

std::optional<Token> Parser::parseIf(std::function<bool(Token)> pred,
                                     std::string expected, bool throws) {
    if (pred(peek())) {
        return consume();
    } else {
        if (throws) {
            throw CodeError{CodeError::Type::PARSER_EXPECTED, peek().span, expected};
        }
        return {};
    }
}

std::optional<Token> Parser::parseText(std::string_view s, bool throws) {
    return parseIf([s](Token tok) { return tok.isStr(s); }, util::quoted(s), throws);
}

std::optional<Token> Parser::parseChar(char c, bool throws) {
    return parseIf([c](Token tok) { return tok.isChar(c); }, util::quoted(c), throws);
}

std::optional<Token> Parser::parseInt(bool throws) {
    return parseIf([](Token tok) { return tok.isInt(); }, "non-negative integer", throws);
}

std::vector<Token> Parser::parseSet(std::function<std::optional<Token>()> pred) {
    auto bg = peek();
    std::vector<Token> ret;
    ret.push_back(parseChar('{').value());
    for (;;) {
        if (peek(-1) == bg && parseChar('}', false)) { break; }
        ret.push_back(pred().value());
        if (auto c = parseChar('}', false)) {
            ret.push_back(c.value());
            break;
        } else if (auto c = parseChar(',', false)) {
            ret.push_back(c.value());
            continue;
        } else {
            throw CodeError{CodeError::Type::PARSER_UNCLOSED_SET,
                            code->span(bg.span, peek(-1).span)};
        }
    }
    return ret;
}

void Parser::parseQ() {
    parseText("#Q");
    parseChar('=');
    tm->setStates(parseSet([this]() {
        return parseIf([](Token tok) { return tok.isID(); }, "identifier");
    }));
}

void Parser::parseS() {
    parseText("#S");
    parseChar('=');
    tm->setInputSymbols(parseSet([this]() {
        return parseIf(
            [](Token tok) { return tok.isValidChar() && !tok.isChar('_'); },
            "ASCII graphic character except for ';', ',', '{', '}', '*' and '_'");
    }));
}

void Parser::parseG() {
    parseText("#G");
    parseChar('=');
    tm->setTapeSymbols(parseSet([this]() {
        return parseIf([](Token tok) { return tok.isValidChar(); },
                       "ASCII graphic character except for ';', ',', '{', '}' and '*'");
    }));
}

void Parser::parseq0() {
    parseText("#q0");
    parseChar('=');
    tm->setInitialState(
        parseIf([](Token tok) { return tok.isID(); }, "identifier").value());
}

void Parser::parseB() {
    parseText("#B");
    parseChar('=');
    tm->setBlankSymbol(parseChar('_').value());
}

void Parser::parseF() {
    parseText("#F");
    parseChar('=');
    tm->setFinalStates(parseSet([this]() {
        return parseIf([](Token tok) { return tok.isID(); }, "identifier");
    }));
}

void Parser::parseN() {
    parseText("#N");
    parseChar('=');
    tm->setNumTapes(parseInt().value());
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
