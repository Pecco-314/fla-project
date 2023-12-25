#ifndef __LEXER_H__
#define __LEXER_H__

#include "code.h"
#include "token.h"
#include <memory>

class Lexer {
  private:
    enum class Status;
    Code::Span span;
    Status status;
    std::vector<Token> tokens;

  public:
    Lexer(std::shared_ptr<Code> code);
    void skip();
    void skipLine();
    void extend();
    char chr() const;
    std::string spanStr() const;
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
