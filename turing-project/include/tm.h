#ifndef __TURING_H__
#define __TURING_H__

#include "code.h"
#include "token.h"
#include <memory>
#include <set>
#include <string_view>

class TuringMachine {
  private:
    std::vector<Token> state_tokens;
    std::vector<Token> input_symbol_tokens;
    std::vector<Token> tape_symbol_tokens;
    Token initial_state_token;
    Token blank_symbol_token;
    std::vector<Token> final_state_tokens;
    Token num_tapes_token;

    void validate() const;

  public:
    std::set<std::string> Q; // states
    std::set<char> S;        // input symbols
    std::set<char> G;        // tape symbols
    std::string q0;          // initial state
    char B = '_';            // blank symbol
    std::set<std::string> F; // final states
    unsigned int N;          // number of tapes

    void parse(std::shared_ptr<Code> code);
    void run(std::string_view input, bool verbose = false);

    void setStates(const std::vector<Token> &tokens);
    void setInputSymbols(const std::vector<Token> &tokens);
    void setTapeSymbols(const std::vector<Token> &tokens);
    void setInitialState(const Token &token);
    void setBlankSymbol(const Token &token);
    void setFinalStates(const std::vector<Token> &tokens);
    void setNumTapes(const Token &token);
};

#endif
