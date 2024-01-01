#include "tm.h"
#include "error.h"
#include "parser.h"
#include "str_literals.h"
#include "util.hpp"
#include <algorithm>
#include <iostream>

using namespace str_literals;

Transition::Transition(std::string_view old_state, std::string_view old_symbols,
                       std::string_view new_symbols, std::string_view direction,
                       std::string_view new_state)
    : old_state(old_state), old_symbols(old_symbols), new_symbols(new_symbols),
      direction(direction), new_state(new_state) {}

bool Transition::operator==(const Transition &other) const {
    return old_state == other.old_state && old_symbols == other.old_symbols &&
           new_symbols == other.new_symbols && direction == other.direction &&
           new_state == other.new_state;
}

bool Transition::operator!=(const Transition &other) const {
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const Transition &t) {
    return os << t.old_state << " " << t.old_symbols << " " << t.new_symbols << " "
              << t.direction << " " << t.new_state;
}

void TuringMachine::parse(std::shared_ptr<Code> code) {
    Parser parser(code, this);
    parser.parse();
    validate();
}

void TuringMachine::setStates(const std::vector<Token> &tokens) {
    state_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        Q.emplace(tokens[i].strval());
    }
}

void TuringMachine::setInputSymbols(const std::vector<Token> &tokens) {
    input_symbol_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        S.emplace(tokens[i].charval());
    }
}

void TuringMachine::setTapeSymbols(const std::vector<Token> &tokens) {
    tape_symbol_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        G.emplace(tokens[i].charval());
    }
}

void TuringMachine::setInitialState(const Token &token) {
    initial_state_token = token;
    q0 = token.strval();
}

void TuringMachine::setBlankSymbol(const Token &token) {
    blank_symbol_token = token;
    B = token.charval();
}

void TuringMachine::setFinalStates(const std::vector<Token> &tokens) {
    final_state_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        F.emplace(tokens[i].strval());
    }
}

void TuringMachine::setNumTapes(const Token &token) {
    num_tapes_token = token;
    N = token.intval();
}

void TuringMachine::addTransition(std::array<Token, 5> tokens) {
    for (auto &&tok : tokens) {
        transition_tokens.emplace_back(tok);
    }
    delta.emplace_back(tokens[0].strval(), tokens[1].strval(), tokens[2].strval(),
                       tokens[3].strval(), tokens[4].strval());
}

void TuringMachine::validate() const {
    auto &&code = initial_state_token.span.code;
    if (!G.count('_')) {
        throw CodeError{
            CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET,
            code->span(tape_symbol_tokens.front().span, tape_symbol_tokens.back().span)};
    }
    if (!std::includes(G.begin(), G.end(), S.begin(), S.end())) {
        throw CodeError{
            CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET,
            code->span(input_symbol_tokens.front().span,
                       input_symbol_tokens.back().span)};
    }
    if (!Q.count(q0)) {
        throw CodeError{CodeError::Type::VALIDATOR_INVALID_STATE,
                        initial_state_token.span, INITIAL_STATE};
    }
    if (!std::includes(Q.begin(), Q.end(), F.begin(), F.end())) {
        throw CodeError{
            CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES,
            code->span(final_state_tokens.front().span, final_state_tokens.back().span)};
    }
    auto &&D = transition_tokens;
    for (int i = 0; i < (int)D.size(); i += 5) {
        if (!Q.count(D[i].strval())) {
            throw CodeError{CodeError::Type::VALIDATOR_INVALID_STATE, D[i].span,
                            util::quoted(D[i].span.str())};
        }
        if (!Q.count(D[i + 4].strval())) {
            throw CodeError{CodeError::Type::VALIDATOR_INVALID_STATE, D[i + 4].span,
                            util::quoted(D[i + 4].span.str())};
        }
        for (int j = 1; j <= 3; ++j) {
            if (D[j].strval().length() != N) {
                throw CodeError{CodeError::Type::VALIDATOR_TRASITION_ITEM_INVALID_LENGTH,
                                D[j].span, std::to_string(N)};
            }
        }
        for (auto it = D[i + 1].span.begin(); it != D[i + 1].span.end(); ++it) {
            if (!G.count(*it) && *it != '*') {
                throw CodeError{
                    CodeError::Type::VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD,
                    it.span()};
            }
        }
        for (auto it = D[i + 2].span.begin(); it != D[i + 2].span.end(); ++it) {
            if (!G.count(*it) && *it != '*') {
                throw CodeError{
                    CodeError::Type::VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD,
                    it.span()};
            }
        }
        for (auto it = D[i + 3].span.begin(); it != D[i + 3].span.end(); ++it) {
            if (*it != 'l' && *it != 'r' && *it != '*') {
                throw CodeError{CodeError::Type::VALIDATOR_INVALID_DIRECTION, it.span()};
            }
        }
    }
}

void TuringMachine::run(std::string_view input, bool verbose) {
    if (verbose) { std::cout << "Input: " << input << std::endl; }
    // TODO: Implement this function
}
