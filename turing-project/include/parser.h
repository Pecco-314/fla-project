#ifndef __PARSER_H__
#define __PARSER_H__

#include "code.h"
#include "tm.h"
#include <optional>
#include <string>

struct ParseError {
    Code::Cursor st;
    Code::Cursor ed;
    std::string msg;
};

class Parser {
  private:
    Code code;
    TuringMachine *tm;

    void parseSkipped(Code::Cursor& cs0);
    bool parseChar(char c, Code::Cursor& cs0, bool throw_err = false);
    bool parseStr(std::string_view str, Code::Cursor& cs0, bool throw_err = false);
    std::optional<std::string> parseId(Code::Cursor& cs0, bool throw_err = false);
    void parseQ(Code::Cursor& cs0);

  public:
    Parser(std::string_view path, TuringMachine *tm);
    void parse();
};

#endif
