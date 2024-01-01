#ifndef __PARSER_H__
#define __PARSER_H__

#include "code.h"
#include "tm.h"
#include "token.h"
#include <functional>

class Parser {
  private:
    std::shared_ptr<Code> code;
    std::vector<Token> tokens;
    std::vector<Token>::iterator it;
    TuringMachine *tm;

    std::optional<Token> parseIf(std::function<bool(Token)> pred, std::string expected,
                                 bool throws = true);
    std::optional<Token> parseText(std::string_view s, bool throws = true);
    std::optional<Token> parseChar(char c, bool throws = true);
    std::optional<Token> parseInt(bool throws = true);
    std::vector<Token> parseSet(std::function<std::optional<Token>()> pred);
    Token combineTokens();

    void parseQ();
    void parseS();
    void parseG();
    void parseq0();
    void parseB();
    void parseF();
    void parseN();

  public:
    Parser(std::shared_ptr<Code> code, TuringMachine *tm);
    Token peek(int cnt = 0) const;
    void next();
    Token consume();
    Token eofToken() const;
    void parse();
};

#endif
