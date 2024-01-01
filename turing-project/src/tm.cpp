#include "tm.h"
#include "error.h"
#include "parser.h"
#include <algorithm>
#include <iostream>

void TuringMachine::parse(std::shared_ptr<Code> code) {
    Parser parser(code, this);
    parser.parse();
}

void TuringMachine::setStates(std::vector<Token> tokens) {
    state_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        Q.emplace(tokens[i].strval());
    }
}

void TuringMachine::setInputSymbols(std::vector<Token> tokens) {
    input_symbol_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        S.emplace(tokens[i].charval());
    }
}

void TuringMachine::setTapeSymbols(std::vector<Token> tokens) {
    tape_symbol_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        G.emplace(tokens[i].charval());
    }
}

void TuringMachine::setInitialState(Token token) {
    initial_state_token = token;
    q0 = token.strval();
}

void TuringMachine::setBlankSymbol(Token token) {
    blank_symbol_token = token;
    B = token.charval();
}

void TuringMachine::setFinalStates(std::vector<Token> tokens) {
    final_state_tokens = tokens;
    for (int i = 1; i < (int)tokens.size() - 1; i += 2) {
        F.emplace(tokens[i].strval());
    }
}

void TuringMachine::setNumTapes(Token token) {
    num_tapes_token = token;
    N = token.intval();
}

void TuringMachine::validate() const {
    auto &&code = initial_state_token.span.code;
    if (!G.count('_')) {
        throw CodeError{
            CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET,
            code->span(tape_symbol_tokens.front().span, tape_symbol_tokens.back().span)};
    }
    if (!std::includes(G.begin(), G.end(), S.begin(), S.end())) {
        throw CodeError{CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET,
                        code->span(input_symbol_tokens.front().span,
                                   input_symbol_tokens.back().span)};
    }
    if (!Q.count(q0)) {
        throw CodeError{CodeError::Type::VALIDATOR_INVALID_INITIAL_STATE,
                        initial_state_token.span};
    }
    if (!std::includes(Q.begin(), Q.end(), F.begin(), F.end())) {
        throw CodeError{
            CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES,
            code->span(final_state_tokens.front().span, final_state_tokens.back().span)};
    }
}

void TuringMachine::run(std::string_view input, bool verbose) {
    if (verbose) { std::cout << "Input: " << input << std::endl; }
    // TODO: Implement this function
}
