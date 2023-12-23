#include "tm.h"
#include "parser.h"
#include <iostream>

void TuringMachine::parse(std::shared_ptr<Code> code) {
    Parser parser(code, this);
    parser.parse();
}

void TuringMachine::addState(std::string_view name) {
    Q.emplace(name);
}

void TuringMachine::addSymbol(char c) {
    S.emplace(c);
}

void TuringMachine::run(std::string_view input, bool verbose) {
    if (verbose) { std::cout << "Input: " << input << std::endl; }
    // TODO: Implement this function
}
