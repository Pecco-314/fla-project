#ifndef __PARSER_H__
#define __PARSER_H__

#include "code.h"
#include "tm.h"
#include "token.h"

class Parser {
  private:
    std::shared_ptr<Code> code;
    std::vector<Token> tokens;
    std::vector<Token>::iterator it;
    TuringMachine *tm;

    bool parseText(std::string_view s, bool throws = true);
    bool parseChar(char c, bool throws = true);

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
