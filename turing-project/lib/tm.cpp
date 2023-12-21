#include "tm.h"
#include "parser.h"
#include <iostream>

void TuringMachine::parse(std::string_view tm_path) {
    Parser parser(tm_path, this);
    parser.parse();
}

void TuringMachine::run(std::string_view input, bool verbose) {
    if (verbose) {
        std::cout << "Input: " << input << std::endl;
    }
    // TODO: Implement this function
}
