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
            std::cerr << ERR << "Invalid character: " << Util::quoted(*span.begin()) << std::endl;
            break;
        case Type::PARSER_EXPECTED_Q:
            std::cerr << ERR << "Expected #Q" << std::endl;
            break;
        case Type::PARSER_EXPECTED_S:
            std::cerr << ERR << "Expected #S" << std::endl;
            break;
        case Type::PARSER_EXPECTED_G:
            std::cerr << ERR << "Expected #G" << std::endl;
            break;
        case Type::PARSER_EXPECTED_Q0:
            std::cerr << ERR << "Expected #q0" << std::endl;
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
        case Type::PARSER_EXPECTED_VALID_CHAR:
            std::cerr << ERR << "Expected valid character" << std::endl;
            std::cerr << NOTE << "Valid characters are: ASCII graph characters except for `,;{}*`"<< std::endl;
            break;
        case Type::PARSER_UNEXPECTED_UNDERSCORE:
            std::cerr << ERR << "Input alphabet cannot contain underscore" << std::endl; 
            break;
        case Type::PARSER_EMPTY_SET:
            std::cerr << ERR << "Set is empty" << std::endl;
            break;
        case Type::VALIDATOR_MISSING_UNDERSCORE_IN_G:
            std::cerr << ERR << "Tape alphabet must contain underscore" << std::endl;
            break;
        case Type::VALIDATOR_S_NOT_SUBSET_OF_G:
            std::cerr << ERR << "Input alphabet must be a subset of tape alphabet" << std::endl;
            break;
        case Type::VALIDATOR_INVALID_INITIAL_STATE:
            std::cerr << ERR << "Initial state must be a valid state" << std::endl;
            break;
        }
        span.code->printHighlight(span, RED);
    } else {
        std::cerr << "Syntax error" << std::endl;
    }
    exit(1);
}
