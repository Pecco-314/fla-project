#include "turing.h"
#include "constants.h"
#include <iostream>
#include <fstream>

TuringMachine::TuringMachine(const std::string &tm_path, bool verbose) : tm_path(tm_path), verbose(verbose) {
    std::ifstream tmfile(tm_path);
    if (!tmfile) {
        std::cerr << ERR << "Unable to read file ";
        perror(tm_path.c_str());
        exit(1);
    }
}

void TuringMachine::run(const std::string &input) {
    // TODO: Implement this function
}
