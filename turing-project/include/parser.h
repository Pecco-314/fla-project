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

    void parseQ();
    void parseS();

  public:
    Parser(std::shared_ptr<Code> code, TuringMachine *tm);
    Token peek(int cnt = 0) const;
    Token cur();
    void next();
    Token eof() const;
    void parse();
};

#endif
