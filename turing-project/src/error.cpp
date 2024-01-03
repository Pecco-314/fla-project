#include "error.h"
#include "color.h"
#include "str_literals.h"
#include "util.h"
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
        assume(false);
        exit(1);
    }
}

void InputError::log(bool verbose) const {
    if (verbose) {
        std::cerr << INPUT << input << std::endl;
        std::cerr << banner("ERR") << std::endl;
        std::cerr << ERR
                  << format(INPUT_SYMBOL_NOT_DEFINED, quoted(input.substr(index, 1)))
                  << std::endl;
        std::cerr << INPUT << input.substr(0, index)
                  << Painted(input.substr(index, 1), RED | BOLD)
                  << input.substr(index + 1) << std::endl;
        std::cerr << std::string(7 + index, ' ') << Painted("^", RED | BOLD) << std::endl;
        std::cerr << banner("END") << std::endl;
    } else {
        std::cerr << ILLEGAL_INPUT << std::endl;
    }
    exit(1);
}

void CodeError::log(bool verbose) const {
    if (verbose) {
        std::string_view msg;
        switch (type) {
        case Type::LEXER_INVALID_CHAR:
            msg = format(LEXER_INVALID_CHAR, quoted(span.front()));
            break;
        case Type::PARSER_EXPECTED:
            msg = format(PARSER_EXPECTED, info);
            break;
        case Type::PARSER_UNCLOSED_SET:
            msg = PARSER_UNCLOSED_SET;
            break;
        case Type::PARSER_UNEXPECTED_UNDERSCORE:
            msg = PARSER_UNEXPECTED_UNDERSCORE;
            break;
        case Type::PARSER_TRANSITION_TOO_FEW_ITEMS:
            msg = PARSER_TRANSITION_TOO_FEW_ITEMS;
            break;
        case Type::PARSER_TRANSITION_NOT_ON_SAME_LINE:
            msg = PARSER_TRANSITION_NOT_ON_SAME_LINE;
            break;
        case Type::PARSER_EMPTY_SET:
            msg = PARSER_EMPTY_SET;
            break;
        case Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET:
            msg = VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET;
            break;
        case Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET:
            msg = VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET;
            break;
        case Type::VALIDATOR_INVALID_STATE:
            msg = format(VALIDATOR_INVALID_STATE, info);
            break;
        case Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES:
            msg = VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES;
            break;
        case Type::VALIDATOR_TRASITION_ITEM_INVALID_LENGTH:
            msg = format(VALIDATOR_TRASITION_ITEM_INVALID_LENGTH, info);
            break;
        case Type::VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD:
            msg = format(VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD,
                         quoted(span.front()));
            break;
        case Type::VALIDATOR_INVALID_DIRECTION:
            msg = format(VALIDATOR_INVALID_DIRECTION, quoted(span.front()));
            break;
        default:
            assume(false);
            exit(1);
        }
        span.code->printMessages(0, msg, span);
    } else {
        std::cerr << SYNTAX_ERROR;
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
    case CodeError::Type::PARSER_TRANSITION_TOO_FEW_ITEMS:
        return os << "PARSER_TRANSITION_TOO_FEW_ITEMS";
    case CodeError::Type::PARSER_TRANSITION_NOT_ON_SAME_LINE:
        return os << "PARSER_TRANSITION_NOT_ON_SAME_LINE";
    case CodeError::Type::PARSER_EMPTY_SET:
        return os << "PARSER_EMPTY_SET";
    case CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET:
        return os << "VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET";
    case CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET:
        return os << "VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET";
    case CodeError::Type::VALIDATOR_INVALID_STATE:
        return os << "VALIDATOR_INVALID_STATE";
    case CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES:
        return os << "VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES";
    case CodeError::Type::VALIDATOR_TRASITION_ITEM_INVALID_LENGTH:
        return os << "VALIDATOR_TRASITION_ITEM_INVALID_LENGTH";
    case CodeError::Type::VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD:
        return os << "VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD";
    case CodeError::Type::VALIDATOR_INVALID_DIRECTION:
        return os << "VALIDATOR_INVALID_DIRECTION";
    default:
        assume(false);
        exit(1);
    }
}
