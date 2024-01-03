#ifndef __STR_LITERALS_H__
#define __STR_LITERALS_H__

#include <string>

namespace str_literals {

const inline std::string USAGE = "Usage: turing [-v|--verbose] [-h|--help] [-c|--color] <tm_path> <input>";
const inline std::string INVALID_OPTION = "Invalid option: {}";
const inline std::string TOO_MANY_ARGS = "Too many arguments";
const inline std::string TOO_FEW_ARGS = "Too few arguments";
const inline std::string FILE_NOT_EXIST = "{} does not exist";
const inline std::string NOT_A_REGULAR_FILE = "{} is not a regular file";
const inline std::string UNABLE_TO_READ = "Unable to read {}";
const inline std::string LEXER_INVALID_CHAR = "Invalid character: {}";
const inline std::string PARSER_EXPECTED = "Expected {}";
const inline std::string PARSER_UNEXPECTED_UNDERSCORE = "Input alphabet cannot contain underscore";
const inline std::string PARSER_UNCLOSED_SET = "Unclosed set";
const inline std::string PARSER_TRANSITION_TOO_FEW_ITEMS = "Transition is expected to have 5 items";
const inline std::string PARSER_TRANSITION_NOT_ON_SAME_LINE = "Transition is not on the same line";
const inline std::string PARSER_EMPTY_SET = "Set is empty";
const inline std::string VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET = "Tape alphabet does not contain underscore";
const inline std::string VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET = "Input alphabet is not a subset of tape alphabet";
const inline std::string VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES = "Final states are not a subset of states";
const inline std::string VALIDATOR_INVALID_STATE = "{} is not found in the set of states";
const inline std::string VALIDATOR_TRASITION_ITEM_INVALID_LENGTH = "Group of symbols or directions is expected to have {} characters";
const inline std::string VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD = "{} is not found in the set of tape symbols, and is not '*'";
const inline std::string VALIDATOR_INVALID_DIRECTION = "{} is not a valid direction";
const inline std::string INPUT_SYMBOL_NOT_DEFINED = "Symbol {} in input is not defined in the set of input symbols";
const inline std::string NON_NEGATIVE_INTEGER = "non-negative integer";
const inline std::string IDENTIFIER = "identifier";
const inline std::string VALID_CHAR = "ASCII graphic character except for ';', ',', '{', '}', '*' and '_'";
const inline std::string VALID_CHAR_OR_UNDERSCORE = "ASCII graphic character except for ';', ',', '{', '}' and '*'";
const inline std::string INITIAL_STATE = "Initial state";
const inline std::string ILLEGAL_INPUT = "Illegal input string";
const inline std::string SYNTAX_ERROR = "Syntax error";

} // namespace str_literals

#endif
