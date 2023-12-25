#ifndef __LEXER_H__
#define __LEXER_H__

#include "code.h"
#include "token.h"
#include <memory>

class Lexer {
  private:
    enum class Status;
    std::shared_ptr<Code> code;
    Code::Cursor st;
    Code::Cursor ed;
    Status status;
    std::vector<Token> tokens;

  public:
    Lexer(std::shared_ptr<Code> code);
    void skip();
    void skipLine();
    void extend();
    char chr() const;
    std::string span() const;
    void store();
    std::vector<Token> lex();
};

enum class Lexer::Status {
    INITIAL,
    WORD,
    PARAM,
    END,
};

#endif
