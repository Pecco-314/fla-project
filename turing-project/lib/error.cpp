#include "error.h"
#include "constants.h"
#include "util.h"
#include <iostream>

void ArgError::log() const {
    switch (type) {
    case Type::INVALID_OPTION:
        std::cerr << ERR << "Invalid option: " << arg << std::endl;
        break;
    case Type::TOO_MANY_ARGS:
        std::cerr << ERR << "Too many arguments" << std::endl;
        break;
    case Type::TOO_FEW_ARGS:
        std::cerr << ERR << "Too few arguments" << std::endl;
        break;
    }
    std::cerr << NOTE;
    Util::usage();
    exit(1);
}

void CodeError::log(bool verbose) const {
    if (verbose) {
        switch (type) {
        case Type::LEXER_INVALID_CHAR:
            std::cerr << ERR << "Invalid character: " << st.span(ed) << std::endl;
            break;
        }
        st.code->printHighlights(st, ed, RED);
    } else {
        std::cerr << "Syntax error" << std::endl;
    }
    exit(1);
}
