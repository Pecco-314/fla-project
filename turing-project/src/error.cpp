#include "error.h"
#include "color.h"
#include "messages.h"
#include "util.hpp"
#include <cassert>
#include <iostream>

using namespace util;
using namespace msg;

void ArgError::log() const {
    std::cerr << ERR;
    switch (type) {
    case Type::INVALID_OPTION:
        std::cerr << format(INVALID_OPTION, arg) << std::endl;
        break;
    case Type::TOO_MANY_ARGS:
        std::cerr << TOO_MANY_ARGS << std::endl;
        break;
    case Type::TOO_FEW_ARGS:
        std::cerr << TOO_FEW_ARGS << std::endl;
        break;
    }
    std::cerr << NOTE << USAGE << std::endl;
    exit(1);
}

std::ostream &operator<<(std::ostream &os, const ArgError::Type &et) {
    switch (et) {
    case ArgError::Type::INVALID_OPTION:
        return os << "INVALID_OPTION";
    case ArgError::Type::TOO_MANY_ARGS:
        return os << "TOO_MANY_ARGS";
    case ArgError::Type::TOO_FEW_ARGS:
        return os << "TOO_FEW_ARGS";
    default:
        assert(false);
    }
}

void CodeError::log(bool verbose) const {
    if (verbose) {
        std::cerr << BOLD << span.begin() << ": " << ERR;
        switch (type) {
        case Type::LEXER_INVALID_CHAR:
            std::cerr << "Invalid character: " << util::quoted(*span.begin())
                      << std::endl;
            break;
        case Type::PARSER_EXPECTED:
            std::cerr << "Expected " <<  expected << std::endl;
            break;
        case Type::PARSER_UNCLOSED_SET:
            std::cerr << "Unclosed set" << std::endl;
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
        case Type::VALIDATOR_F_NOT_SUBSET_OF_Q:
            std::cerr << "Final states must be a subset of states" << std::endl;
            break;
        case Type::VALIDATOR_INVALID_NUM_TAPES:
            std::cerr << "Number of tapes must be a positive integer" << std::endl;
            break;
        }
        span.code->printHighlight(span, RED | BOLD);
    } else {
        std::cerr << "Syntax error" << std::endl;
    }
    exit(1);
}

std::ostream &operator<<(std::ostream &os, const CodeError::Type &e) {
    switch (e) {
    case CodeError::Type::LEXER_INVALID_CHAR:
        return os << "LEXER_INVALID_CHAR";
    case CodeError::Type::PARSER_EXPECTED:
        return os << "PARSER_EXPECTED";
    case CodeError::Type::PARSER_UNCLOSED_SET:
        return os << "PARSER_UNCLOSED_SET";
    case CodeError::Type::PARSER_UNEXPECTED_UNDERSCORE:
        return os << "PARSER_UNEXPECTED_UNDERSCORE";
    case CodeError::Type::PARSER_EMPTY_SET:
        return os << "PARSER_EMPTY_SET";
    case CodeError::Type::PARSER_EXPECTED_UNDERSCORE_AS_BLANK:
        return os << "PARSER_EXPECTED_UNDERSCORE_AS_BLANK";
    case CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_G:
        return os << "VALIDATOR_MISSING_UNDERSCORE_IN_G";
    case CodeError::Type::VALIDATOR_S_NOT_SUBSET_OF_G:
        return os << "VALIDATOR_S_NOT_SUBSET_OF_G";
    case CodeError::Type::VALIDATOR_INVALID_INITIAL_STATE:
        return os << "VALIDATOR_INVALID_INITIAL_STATE";
    case CodeError::Type::VALIDATOR_F_NOT_SUBSET_OF_Q:
        return os << "VALIDATOR_F_NOT_SUBSET_OF_Q";
    case CodeError::Type::VALIDATOR_INVALID_NUM_TAPES:
        return os << "VALIDATOR_INVALID_NUM_TAPES";
    default:
        assert(false);
    }
}
