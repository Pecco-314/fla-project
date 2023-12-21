#include "util.h"
#include <iostream>

void Util::usage() {
    std::cerr << "Usage: turing [-v|--verbose] [-h|--help] <tm_path> <input>"
              << std::endl;
}
