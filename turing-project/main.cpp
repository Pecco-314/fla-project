#include "constants.h"
#include "tm.h"
#include <iostream>

void usage() {
    std::cerr << "Usage: turing [-v|--verbose] [-h|--help] <tm_path> <input>"
              << std::endl;
}

int main(int argc, char *argv[]) {
    std::string tm_path, input;
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            usage();
            exit(0);
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg.size() > 1 && arg[0] == '-') {
            std::cerr << ERR << "Invalid option: " << arg << std::endl;
            std::cerr << NOTE;
            usage();
            exit(1);
        } else if (tm_path.empty()) {
            tm_path = arg;
        } else if (input.empty()) {
            input = arg;
        } else {
            std::cerr << ERR << "Too many arguments" << std::endl;
            std::cerr << NOTE;
            usage();
            exit(1);
        }
    }
    if (tm_path.empty()) {
        std::cerr << ERR << "No tm_path given" << std::endl;
        std::cerr << NOTE;
        usage();
        exit(1);
    } else if (input.empty()) {
        std::cerr << ERR << "No input given" << std::endl;
        std::cerr << NOTE;
        usage();
        exit(1);
    }

    TuringMachine tm;
    tm.parse(tm_path, verbose);
    tm.run(input, verbose);

    return 0;
}
