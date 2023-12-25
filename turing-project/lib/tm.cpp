#include "tm.h"
#include "error.h"
#include "parser.h"
#include <algorithm>
#include <cassert>
#include <iostream>

void TuringMachine::parse(std::shared_ptr<Code> code) {
    Parser parser(code, this);
    parser.parse();
}

void TuringMachine::addState(std::string_view name) {
    Q.emplace(name);
}

void TuringMachine::addInputSymbol(char c) {
    S.emplace(c);
}

void TuringMachine::addTapeSymbol(char c) {
    G.emplace(c);
}

void TuringMachine::setSpan(std::string_view name, Code::Span span) {
    span_map.emplace(name, span);
}

Code::Span TuringMachine::getSpan(std::string_view name) const {
    assert(span_map.find(name) != span_map.end());
    return span_map.find(name)->second;
}

void TuringMachine::validate() const {
    if (!G.count('_')) {
        throw CodeError{CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_G, getSpan("G")};
    }
    if (!std::includes(G.begin(), G.end(), S.begin(), S.end())) {
        throw CodeError{CodeError::Type::VALIDATOR_S_NOT_SUBSET_OF_G, getSpan("S")};
    }
}

void TuringMachine::run(std::string_view input, bool verbose) {
    if (verbose) { std::cout << "Input: " << input << std::endl; }
    // TODO: Implement this function
}
