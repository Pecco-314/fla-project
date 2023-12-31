#include "constants.h"
#include "messages.h"
#include "error.h"
#include "tm.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::string tm_path, input;
    bool verbose = false;
    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                std::cerr << msg::USAGE << std::endl;
                exit(0);
            } else if (arg == "-v" || arg == "--verbose") {
                verbose = true;
            } else if (arg == "-c" || arg == "--color") {
                TermColor::setForceColor();
            } else if (arg.size() > 1 && arg[0] == '-') {
                throw ArgError{ArgError::Type::INVALID_OPTION, arg};
            } else if (tm_path.empty()) {
                tm_path = arg;
            } else if (input.empty()) {
                input = arg;
            } else {
                throw ArgError{ArgError::Type::TOO_MANY_ARGS, arg};
            }
        }
        if (input.empty()) { throw ArgError{ArgError::Type::TOO_FEW_ARGS, ""}; }
        TuringMachine tm;
        auto code = std::make_shared<Code>(tm_path);
        tm.parse(code);
        tm.run(input, verbose);
    } catch (ArgError &e) { e.log(); } catch (CodeError &e) {
        e.log(verbose);
    }
    return 0;
}
