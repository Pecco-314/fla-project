#include "str_literals.h"
#include "error.h"
#include "tm.h"
#include <iostream>

using namespace str_literals;

int main(int argc, char *argv[]) {
    std::string tm_path, input;
    bool read_tm_path = false;
    bool read_input = false;
    bool verbose = false;
    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg == "-h" || arg == "--help") {
                std::cerr << USAGE << std::endl;
                exit(0);
            } else if (arg == "-v" || arg == "--verbose") {
                verbose = true;
            } else if (arg == "-c" || arg == "--color") {
                TermColor::setForceColor();
            } else if (arg.size() > 1 && arg[0] == '-') {
                throw ArgError{ArgError::Type::INVALID_OPTION, arg};
            } else if (!read_tm_path) {
                tm_path = arg;
                read_tm_path = true;
            } else if (!read_input) {
                input = arg;
                read_input = true;
            } else {
                throw ArgError{ArgError::Type::TOO_MANY_ARGS, arg};
            }
        }
        if (!read_input) { throw ArgError{ArgError::Type::TOO_FEW_ARGS, ""}; }
        TuringMachine tm;
        auto code = std::make_shared<Code>(tm_path);
        tm.parse(code);
        tm.run(input, verbose);
    } catch (ArgError &e) {
        e.log();
    } catch (CodeError &e) {
        e.log(verbose);
    } catch (InputError &e) {
        e.log(verbose);
    }
    return 0;
}
