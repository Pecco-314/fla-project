#include "color.h"
#include "error.h"
#include "str_literals.h"
#include "tm.h"
#include "util.hpp"
#include <fstream>

using namespace util;
using namespace str_literals;

std::unique_ptr<TuringMachine> parse(std::string_view text) {
    std::ofstream file("test.tm");
    file << text;
    file.close();
    auto code = std::make_shared<Code>("test.tm");
    auto tm = std::make_unique<TuringMachine>();
    tm->parse(code);
    return tm;
}

int main() {
    TermColor::setForceColor();
    try {
        parse("");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t1_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t1_span", e.span, code->span(0, 0, 0, 0));
        simpletest("t1_expected", e.expected, "\"#Q\"");
    }
    try {
        parse("#");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t2_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t2_span", e.span, code->span(0, 0, 0, 1));
        simpletest("t2_expected", e.expected, "\"#Q\"");
    }
    try {
        parse("#Q");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t3_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t3_span", e.span, code->span(1, 0, 1, 0));
        simpletest("t3_expected", e.expected, "'='");
    }
    try {
        parse("#Q =");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t4_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t4_span", e.span, code->span(1, 0, 1, 0));
        simpletest("t4_expected", e.expected, "'{'");
    }
    try {
        parse("#Q = }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t5_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t5_span", e.span, code->span(0, 5, 0, 6));
        simpletest("t5_expected", e.expected, "'{'");
    }
    try {
        parse("#Q = {");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t6_type", e.type, CodeError::Type::PARSER_UNCLOSED_SET);
        simpletest("t6_span", e.span, code->span(0, 5, 0, 6));
    }
    try {
        parse("#Q = { #S }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t7_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t7_span", e.span, code->span(0, 7, 0, 9));
        simpletest("t7_expected", e.expected, IDENTIFIER);
    }
    try {
        parse("#Q = { q0, }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t8_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t8_span", e.span, code->span(0, 11, 0, 12));
        simpletest("t8_expected", e.expected, IDENTIFIER);
    }
    try {
        parse("#Q = { q0, State_1, 123, hello-world }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t9_type", e.type, CodeError::Type::PARSER_UNCLOSED_SET);
        simpletest("t9_span", e.span, code->span(0, 5, 0, 30));
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t10_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t10_span", e.span, code->span(1, 0, 1, 0));
        simpletest("t10_expected", e.expected, "\"#S\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { s0 }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t11_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t11_span", e.span, code->span(1, 7, 1, 9));
        simpletest("t11_expected", e.expected, VALID_CHAR);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, _ }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t12_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t12_span", e.span, code->span(1, 10, 1, 11));
        simpletest("t12_expected", e.expected, VALID_CHAR);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t13_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t13_span", e.span, code->span(2, 0, 2, 0));
        simpletest("t13_expected", e.expected, "\"#G\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t14_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t14_span", e.span, code->span(3, 0, 3, 0));
        simpletest("t14_expected", e.expected, "\"#q0\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = ?");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t15_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t15_span", e.span, code->span(3, 6, 3, 7));
        simpletest("t15_expected", e.expected, IDENTIFIER);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t16_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t16_span", e.span, code->span(4, 0, 4, 0));
        simpletest("t16_expected", e.expected, "\"#B\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = abcd");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t17_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t17_span", e.span, code->span(4, 5, 4, 9));
        simpletest("t17_expected", e.expected, "'_'");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t18_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t18_span", e.span, code->span(5, 0, 5, 0));
        simpletest("t18_expected", e.expected, "\"#F\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123, # }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t19_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t19_span", e.span, code->span(5, 25, 5, 26));
        simpletest("t19_expected", e.expected, IDENTIFIER);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t20_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t20_span", e.span, code->span(6, 0, 6, 0));
        simpletest("t20_expected", e.expected, "\"#N\"");
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 0");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t21_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t21_span", e.span, code->span(6, 4, 6, 5));
        simpletest("t21_expected", e.expected, NON_NEGATIVE_INTEGER);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = -1");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t22_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t22_span", e.span, code->span(6, 5, 6, 6));
        simpletest("t22_expected", e.expected, NON_NEGATIVE_INTEGER);
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 1, 0 }\n"
              "#G = { 0, 1 }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 2");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t23_type", e.type,
                   CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET);
        simpletest("t23_span", e.span, code->span(2, 5, 2, 13));
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1, \" }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 2");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t24_type", e.type,
                   CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET);
        simpletest("t24_span", e.span, code->span(1, 5, 1, 16));
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = Q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 2");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t25_type", e.type, CodeError::Type::VALIDATOR_INVALID_INITIAL_STATE);
        simpletest("t25_span", e.span, code->span(3, 6, 3, 8));
    }
    try {
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 321 }\n"
              "#N = 2\n");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t26_type", e.type,
                   CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES);
        simpletest("t26_span", e.span, code->span(5, 5, 5, 25));
    }
    auto tm = parse("#Q = { q0, State_1, 123, helloworld }\n"
                    "#S = { 0, 1 }\n"
                    "#G = { 0, 1, _ }\n"
                    "#q0 = q0\n"
                    "#B = _\n"
                    "#F = { q0, State_1, 123 }\n"
                    "#N = 2");
    simpletest("tm_q", tm->Q,
               std::set<std::string>{"q0", "State_1", "123", "helloworld"});
    simpletest("tm_S", tm->S, std::set<char>{'0', '1'});
    simpletest("tm_G", tm->G, std::set<char>{'0', '1', '_'});
    simpletest("tm_q0", tm->q0, "q0");
    simpletest("tm_B", tm->B, '_');
    simpletest("tm_F", tm->F, std::set<std::string>{"q0", "State_1", "123"});
    simpletest("tm_N", tm->N, 2);

    return 0;
}
