#ifndef __LEXER_H__
#define __LEXER_H__

#include "code.h"
#include "error.h"
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

    void skip();
    void skipLine();
    void extend();
    char chr() const;
    std::string span() const;
    CodeError mkerr(CodeError::Type) const;
    bool eof() const;
    void store();

  public:
    Lexer(std::shared_ptr<Code> code);
    std::vector<Token> lex();
};

enum class Lexer::Status {
    INITIAL,
    WORD,
    PARAM,
};

#endif
