#include "error.h"
#include "constants.h"
#include "util.h"
#include <iostream>

void ArgError::log() const {
    std::cerr << ERR;
    switch (type) {
    case Type::INVALID_OPTION:
        std::cerr << "Invalid option: " << arg << std::endl;
        break;
    case Type::TOO_MANY_ARGS:
        std::cerr << "Too many arguments" << std::endl;
        break;
    case Type::TOO_FEW_ARGS:
        std::cerr << "Too few arguments" << std::endl;
        break;
    }
    std::cerr << NOTE;
    Util::usage();
    exit(1);
}

void CodeError::log(bool verbose) const {
    if (verbose) {
        std::cerr << BOLD << span.code->path() << ":" << span.st_lno + 1 << ":" << span.st_cno + 1 << ": " << ERR;
        switch (type) {
        case Type::LEXER_INVALID_CHAR:
            std::cerr << "Invalid character: " << Util::quoted(*span.begin()) << std::endl;
            break;
        case Type::PARSER_EXPECTED_Q:
            std::cerr << "Expected #Q" << std::endl;
            break;
        case Type::PARSER_EXPECTED_S:
            std::cerr << "Expected #S" << std::endl;
            break;
        case Type::PARSER_EXPECTED_G:
            std::cerr << "Expected #G" << std::endl;
            break;
        case Type::PARSER_EXPECTED_Q0:
            std::cerr << "Expected #q0" << std::endl;
            break;
        case Type::PARSER_EXPECTED_B:
            std::cerr << "Expected #B" << std::endl;
            break;
        case Type::PARSER_EXPECTED_EQUAL:
            std::cerr << "Expected '='" << std::endl;
            break;
        case Type::PARSER_EXPECTED_LBRACE:
            std::cerr << "Expected '{'" << std::endl;
            break;
        case Type::PARSER_EXPECTED_ID:
            std::cerr << "Expected identifier" << std::endl;
            break;
        case Type::PARSER_UNCLOSED_SET:
            std::cerr << "Unclosed set" << std::endl;
            break;
        case Type::PARSER_EXPECTED_VALID_CHAR:
            std::cerr << "Expected valid character (valid characters are: ASCII graph characters except for `,;{}*`)"<< std::endl;
            break;
        case Type::PARSER_UNEXPECTED_UNDERSCORE:
            std::cerr << "Input alphabet cannot contain underscore" << std::endl; 
            break;
        case Type::PARSER_EMPTY_SET:
            std::cerr << "Set is empty" << std::endl;
            break;
        case Type::PARSER_EXPECTED_UNDERSCORE_AS_BLANK:
            std::cerr << "Expected underscore as blank character" << std::endl;
            break;
        case Type::VALIDATOR_MISSING_UNDERSCORE_IN_G:
            std::cerr << "Tape alphabet must contain underscore" << std::endl;
            break;
        case Type::VALIDATOR_S_NOT_SUBSET_OF_G:
            std::cerr << "Input alphabet must be a subset of tape alphabet" << std::endl;
            break;
        case Type::VALIDATOR_INVALID_INITIAL_STATE:
            std::cerr << "Initial state must be a valid state" << std::endl;
            break;
        }
        span.code->printHighlight(span, RED BOLD);
    } else {
        std::cerr << "Syntax error" << std::endl;
    }
    exit(1);
}
