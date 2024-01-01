#include "error.h"
#include "color.h"
#include "str_literals.h"
#include "util.hpp"
#include <cassert>
#include <iostream>

using namespace util;
using namespace str_literals;

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
            std::cerr << format(LEXER_INVALID_CHAR, quoted(span.front())) << std::endl;
            break;
        case Type::PARSER_EXPECTED:
            std::cerr << format(PARSER_EXPECTED, info) << std::endl;
            break;
        case Type::PARSER_UNCLOSED_SET:
            std::cerr << PARSER_UNCLOSED_SET << std::endl;
            break;
        case Type::PARSER_UNEXPECTED_UNDERSCORE:
            std::cerr << PARSER_UNEXPECTED_UNDERSCORE << std::endl;
            break;
        case Type::PARSER_EMPTY_SET:
            std::cerr << PARSER_EMPTY_SET << std::endl;
            break;
        case Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET:
            std::cerr << VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET << std::endl;
            break;
        case Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET:
            std::cerr << VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET << std::endl;
            break;
        case Type::VALIDATOR_INVALID_STATE:
            std::cerr << format(VALIDATOR_INVALID_STATE, info) << std::endl;
            break;
        case Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES:
            std::cerr << VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES << std::endl;
            break;
        default:
            assert(false);
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
    case CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET:
        return os << "VALIDATOR_MISSING_UNDERSCORE_IN_G";
    case CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET:
        return os << "VALIDATOR_S_NOT_SUBSET_OF_G";
    case CodeError::Type::VALIDATOR_INVALID_STATE:
        return os << "VALIDATOR_INVALID_STATE";
    case CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES:
        return os << "VALIDATOR_F_NOT_SUBSET_OF_Q";
    default:
        assert(false);
    }
}
