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
            std::cerr << ERR << "Invalid character: " << Util::quoted(*st) << std::endl;
            break;
        case Type::PARSER_EXPECTED_Q:
            std::cerr << ERR << "Expected #Q" << std::endl;
            break;
        case Type::PARSER_EXPECTED_EQUAL:
            std::cerr << ERR << "Expected '='" << std::endl;
            break;
        case Type::PARSER_EXPECTED_LBRACE:
            std::cerr << ERR << "Expected '{'" << std::endl;
            break;
        case Type::PARSER_EXPECTED_ID:
            std::cerr << ERR << "Expected identifier" << std::endl;
            break;
        case Type::PARSER_UNCLOSED_SET:
            std::cerr << ERR << "Unclosed set" << std::endl;
            break;
        case Type::PARSER_EXPECTED_S:
            std::cerr << ERR << "Expected #S" << std::endl;
            break;
        case Type::PARSER_EXPECTED_VALID_CHAR:
            std::cerr << ERR << "Expected valid character" << std::endl;
            std::cerr << NOTE << "Valid characters are: ASCII graph characters except for `,;{}*_`"<< std::endl;
            break;
        case Type::PARSER_EMPTY_SET:
            std::cerr << ERR << "Set is empty" << std::endl;
            break;
        }
        st.code->printHighlight(st, ed, RED);
    } else {
        std::cerr << "Syntax error" << std::endl;
    }
    exit(1);
}
