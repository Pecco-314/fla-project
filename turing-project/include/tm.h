#ifndef __TURING_H__
#define __TURING_H__

#include "code.h"
#include <map>
#include <memory>
#include <set>
#include <string_view>

class TuringMachine {
    friend class Parser;

  private:
    std::set<std::string> Q; // states
    std::set<char> S;        // input symbols
    std::set<char> G;        // tape symbols
    std::string q0;          // initial state
    std::map<std::string, Code::Span, std::less<>> span_map;

  public:
    void parse(std::shared_ptr<Code> code);
    void validate() const;
    void run(std::string_view input, bool verbose = false);
    void addState(std::string_view name);
    void addInputSymbol(char c);
    void addTapeSymbol(char c);
    void setInitialState(std::string_view name);
    void setSpan(std::string_view name, Code::Span span);
    Code::Span getSpan(std::string_view name) const;
};

#endif
