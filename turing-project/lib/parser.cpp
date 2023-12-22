#include "parser.h"
#include "constants.h"
#include <cctype>
#include <iostream>


void Parser::parseSkipped(Code::Cursor &cs0) {
    auto cs = cs0;
    while (!cs.eof()) {
        if (isspace(*cs)) {
            ++cs;
        } else if (*cs == ';') {
            cs.skipLine();
        } else {
            break;
        }
    }
    cs0 = cs;
}

bool Parser::parseChar(char c, Code::Cursor &cs0, bool throw_err) {
    auto cs = cs0;
    if (!cs.eof() && *cs++ == c) {
        cs0 = cs;
        return true;
    } else {
        if (throw_err) {
            throw ParseError{cs0, cs, "Expected '" + std::string(1, c) + "'"};
        }
        return false;
    }
}

bool Parser::parseStr(std::string_view str, Code::Cursor &cs0, bool throw_err) {
    Code::InlineCursor ic = cs0;
    for (auto c : str) {
        if (!ic.eol() && *ic == c) {
            ++ic;
        } else {
            if (throw_err) {
                throw ParseError{cs0, ic, "Expected '" + std::string(str) + "'"};
            }
            return false;
        }
    }
    cs0 = ic;
    return true;
}

std::optional<std::string> Parser::parseId(Code::Cursor &cs0, bool throw_err) {
    std::string id;
    auto cs = cs0;
    while (!cs.eof() && (isalnum(*cs) || *cs == '_')) {
        id += *cs++;
    }
    if (id.empty()) {
        if (throw_err) {
            throw ParseError{cs0, cs, "Expected identifier consisting of alphanumeric characters and/or underscores"};
        }
        return std::nullopt;
    } else {
        cs0 = cs;
        return id;
    }
}

void Parser::parseQ(Code::Cursor &cs0) {
    auto cs = cs0;
    parseStr("#Q", cs, true);
    parseSkipped(cs);
    parseChar('=', cs, true);
    parseSkipped(cs);
    auto quote_cs = cs;
    parseChar('{', cs, true);
    for (;;) {
        parseSkipped(cs);
        auto &&id = parseId(cs, true);
        parseSkipped(cs);
        tm->Q.push_back(id.value());
        if (parseChar('}', cs, false)) {
            break;
        } else if (parseChar(',', cs, false)) {
            continue;
        } else {
            throw ParseError{quote_cs, cs, "Set must be closed by '}'"};
        }
    }
    cs = cs0;
}

void Parser::parse(bool verbose) {
    try {
        auto cs = code.begin();
        parseQ(cs);
    } catch (const ParseError &err) {
        if (verbose) {
            std::cerr << ERR << "Syntax error at " << UL << code.path() << ":" << err.st.lno + 1 << ":"
                      << err.st.cno + 1 << RESET << ": " << err.msg << std::endl;
            code.printHighlights(err.st, err.ed, RED);
        } else {
            std::cerr << "syntax error" << std::endl;
        }
        exit(1);
    }
Parser::Parser(std::shared_ptr<Code> code, TuringMachine* tm)
    : code(code), tm(tm) {}
}
