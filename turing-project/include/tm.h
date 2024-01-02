#ifndef __TURING_H__
#define __TURING_H__

#include "code.h"
#include "tape.h"
#include "token.h"
#include <memory>
#include <set>
#include <string_view>

struct Transition {
    std::string old_state;
    std::string old_symbols;
    std::string new_symbols;
    std::string direction;
    std::string new_state;
    Transition(std::string_view old_state, std::string_view old_symbols,
               std::string_view new_symbols, std::string_view direction,
               std::string_view new_state);
    bool operator==(const Transition &other) const;
    bool operator!=(const Transition &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Transition &t);
};

class TuringMachine {
  private:
    std::vector<Token> state_tokens;
    std::vector<Token> input_symbol_tokens;
    std::vector<Token> tape_symbol_tokens;
    Token initial_state_token;
    Token blank_symbol_token;
    std::vector<Token> final_state_tokens;
    Token num_tapes_token;
    std::vector<Token> transition_tokens;

    std::vector<Tape> tapes;
    std::string state;
    bool acc;

    void validate() const;
    void validateInput(std::string_view input) const;
    void init(std::string_view input);
    bool step();

  public:
    std::set<std::string> Q;       // states
    std::set<char> S;              // input symbols
    std::set<char> G;              // tape symbols
    std::string q0;                // initial state
    char B = '_';                  // blank symbol
    std::set<std::string> F;       // final states
    unsigned int N;                // number of tapes
    std::vector<Transition> delta; // transitions

    void parse(std::shared_ptr<Code> code);
    void run(std::string_view input, bool verbose = false);

    void setStates(const std::vector<Token> &tokens);
    void setInputSymbols(const std::vector<Token> &tokens);
    void setTapeSymbols(const std::vector<Token> &tokens);
    void setInitialState(const Token &token);
    void setBlankSymbol(const Token &token);
    void setFinalStates(const std::vector<Token> &tokens);
    void setNumTapes(const Token &token);
    void addTransition(std::array<Token, 5> tokens);
};

#endif
