#include "parser.h"
#include "error.h"
#include "lexer.h"
#include "str_literals.h"
#include "util.h"

using namespace str_literals;

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
    return parseIf([](Token tok) { return tok.isInt(); }, NON_NEGATIVE_INTEGER, throws);
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

Token Parser::combineTokens() {
    auto token = consume();
    while (!token.span.eof() && token.span.nextTo(peek().span)) {
        token += consume();
    }
    return token;
}

std::optional<std::array<Token, 5>> Parser::parseTransition() {
    std::array<Token, 5> tokens;
    for (int i = 0; i < 5; ++i) {
        tokens[i] = combineTokens();
    }
    if (tokens[0].span.eof()) { return {}; }
    if (!tokens[0].span.eof() && tokens[4].span.eof()) {
        for (int i = 3; i >= 0; --i) {
            if (!tokens[i].span.eof()) {
                throw CodeError{CodeError::Type::PARSER_TRANSITION_TOO_FEW_ITEMS,
                                code->span(tokens[0].span, tokens[i].span)};
            }
        }
    }
    if (tokens[0].span.st_lno != tokens[4].span.ed_lno) {
        throw CodeError{CodeError::Type::PARSER_TRANSITION_NOT_ON_SAME_LINE,
                        code->span(tokens[0].span, tokens[4].span)};
    }
    return tokens;
}

void Parser::parseQ() {
    parseText("#Q");
    parseChar('=');
    tm->setStates(parseSet(
        [this]() { return parseIf([](Token tok) { return tok.isID(); }, IDENTIFIER); }));
}

void Parser::parseS() {
    parseText("#S");
    parseChar('=');
    tm->setInputSymbols(parseSet([this]() {
        return parseIf([](Token tok) { return tok.isValidChar() && !tok.isChar('_'); },
                       VALID_CHAR);
    }));
}

void Parser::parseG() {
    parseText("#G");
    parseChar('=');
    tm->setTapeSymbols(parseSet([this]() {
        return parseIf([](Token tok) { return tok.isValidChar(); },
                       VALID_CHAR_OR_UNDERSCORE);
    }));
}

void Parser::parseq0() {
    parseText("#q0");
    parseChar('=');
    tm->setInitialState(
        parseIf([](Token tok) { return tok.isID(); }, IDENTIFIER).value());
}

void Parser::parseB() {
    parseText("#B");
    parseChar('=');
    tm->setBlankSymbol(parseChar('_').value());
}

void Parser::parseF() {
    parseText("#F");
    parseChar('=');
    tm->setFinalStates(parseSet(
        [this]() { return parseIf([](Token tok) { return tok.isID(); }, IDENTIFIER); }));
}

void Parser::parseN() {
    parseText("#N");
    parseChar('=');
    tm->setNumTapes(parseInt().value());
}

void Parser::parseDelta() {
    for (;;) {
        auto t = parseTransition();
        if (t.has_value()) {
            tm->addTransition(t.value());
        } else {
            break;
        }
    }
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
    parseDelta();
}
