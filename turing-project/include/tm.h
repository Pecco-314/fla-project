#ifndef __TURING_H__
#define __TURING_H__

#include "code.h"
#include <memory>
#include <string_view>
#include <set>
#include <map>

class TuringMachine {
    friend class Parser;

  private:
    std::set<std::string> Q;
    std::set<char> S;
    std::set<char> G;
    std::map<std::string, Code::Span, std::less<>> span_map;

  public:
    void parse(std::shared_ptr<Code> code);
    void run(std::string_view input, bool verbose = false);
    void addState(std::string_view name);
    void addInputSymbol(char c);
    void addTapeSymbol(char c);
    void setSpan(std::string_view name, Code::Span span);
    Code::Span getSpan(std::string_view name) const;
};

#endif
