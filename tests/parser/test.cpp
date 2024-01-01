#include "color.h"
#include "error.h"
#include "str_literals.h"
#include "tm.h"
#include "util.h"
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
    TEST_ERROR("t1", parse(""), CodeError, {
        simpletest("t1_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t1_span", e.span, e.span.code->span(0, 0, 0, 0));
        simpletest("t1_expected", e.info, "\"#Q\"");
    })
    TEST_ERROR("t2", parse("#"), CodeError, {
        simpletest("t2_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t2_span", e.span, e.span.code->span(0, 0, 0, 1));
        simpletest("t2_expected", e.info, "\"#Q\"");
    })
    TEST_ERROR("t3", parse("#Q"), CodeError, {
        simpletest("t3_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t3_span", e.span, e.span.code->span(1, 0, 1, 0));
        simpletest("t3_expected", e.info, "'='");
    })
    TEST_ERROR("t4", parse("#Q ="), CodeError, {
        simpletest("t4_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t4_span", e.span, e.span.code->span(1, 0, 1, 0));
        simpletest("t4_expected", e.info, "'{'");
    })
    TEST_ERROR("t5", parse("#Q = }"), CodeError, {
        simpletest("t5_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t5_span", e.span, e.span.code->span(0, 5, 0, 6));
        simpletest("t5_expected", e.info, "'{'");
    })
    TEST_ERROR("t6", parse("#Q = {"), CodeError, {
        simpletest("t6_type", e.type, CodeError::Type::PARSER_UNCLOSED_SET);
        simpletest("t6_span", e.span, e.span.code->span(0, 5, 0, 6));
    })
    TEST_ERROR("t7", parse("#Q = { #S }"), CodeError, {
        simpletest("t7_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t7_span", e.span, e.span.code->span(0, 7, 0, 9));
        simpletest("t7_expected", e.info, IDENTIFIER);
    })
    TEST_ERROR("t8", parse("#Q = { q0, }"), CodeError, {
        simpletest("t8_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t8_span", e.span, e.span.code->span(0, 11, 0, 12));
        simpletest("t8_expected", e.info, IDENTIFIER);
    })
    TEST_ERROR("t9", parse("#Q = { q0, State_1, 123, hello-world"), CodeError, {
        simpletest("t9_type", e.type, CodeError::Type::PARSER_UNCLOSED_SET);
        simpletest("t9_span", e.span, e.span.code->span(0, 5, 0, 30));
    })
    std::string basic = "#Q = { q0, State_1, 123, helloworld }\n";
    TEST_ERROR("t10", parse(basic), CodeError, {
        simpletest("t10_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t10_span", e.span, e.span.code->span(1, 0, 1, 0));
        simpletest("t10_expected", e.info, "\"#S\"");
    })
    TEST_ERROR("t11", parse(basic + "#S = { s0 }"), CodeError, {
        simpletest("t11_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t11_span", e.span, e.span.code->span(1, 7, 1, 9));
        simpletest("t11_expected", e.info, VALID_CHAR);
    })
    basic += "#S = { 0, 1 }\n";
    TEST_ERROR("t13", parse(basic), CodeError, {
        simpletest("t13_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t13_span", e.span, e.span.code->span(2, 0, 2, 0));
        simpletest("t13_expected", e.info, "\"#G\"");
    })
    basic += "#G = { 0, 1, _ }\n";
    TEST_ERROR("t14", parse(basic), CodeError, {
        simpletest("t14_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t14_span", e.span, e.span.code->span(3, 0, 3, 0));
        simpletest("t14_expected", e.info, "\"#q0\"");
    })
    TEST_ERROR("t15", parse(basic + "#q0 = ?"), CodeError, {
        simpletest("t15_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t15_span", e.span, e.span.code->span(3, 6, 3, 7));
        simpletest("t15_expected", e.info, IDENTIFIER);
    })
    basic += "#q0 = q0\n";
    TEST_ERROR("t16", parse(basic), CodeError, {
        simpletest("t16_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t16_span", e.span, e.span.code->span(4, 0, 4, 0));
        simpletest("t16_expected", e.info, "\"#B\"");
    })
    TEST_ERROR("t17", parse(basic + "#B = abcd"), CodeError, {
        simpletest("t17_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t17_span", e.span, e.span.code->span(4, 5, 4, 9));
        simpletest("t17_expected", e.info, "'_'");
    })
    basic += "#B = _\n";
    TEST_ERROR("t18", parse(basic), CodeError, {
        simpletest("t18_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t18_span", e.span, e.span.code->span(5, 0, 5, 0));
        simpletest("t18_expected", e.info, "\"#F\"");
    })
    TEST_ERROR("t19", parse(basic + "#F = { q0, State_1, 123, # }"), CodeError, {
        simpletest("t19_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t19_span", e.span, e.span.code->span(5, 25, 5, 26));
        simpletest("t19_expected", e.info, IDENTIFIER);
    })
    basic += "#F = { q0, State_1, 123 }\n";
    TEST_ERROR("t20", parse(basic), CodeError, {
        simpletest("t20_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t20_span", e.span, e.span.code->span(6, 0, 6, 0));
        simpletest("t20_expected", e.info, "\"#N\"");
    })
    TEST_ERROR("t21", parse(basic + "#N = *"), CodeError, {
        simpletest("t21_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t21_span", e.span, e.span.code->span(6, 5, 6, 6));
        simpletest("t21_expected", e.info, NON_NEGATIVE_INTEGER);
    })
    TEST_ERROR("t22", parse(basic + "#N = -1"), CodeError, {
        simpletest("t22_type", e.type, CodeError::Type::PARSER_EXPECTED);
        simpletest("t22_span", e.span, e.span.code->span(6, 5, 6, 6));
        simpletest("t22_expected", e.info, NON_NEGATIVE_INTEGER);
    })
    basic += "#N = 2\n";
    TEST_ERROR(
        "t23",
        parse("#Q = { q0, State_1, 123, helloworld }\n"
            "#S = { 1, 0 }\n"
            "#G = { 0, 1 }\n"
            "#q0 = q0\n"
            "#B = _\n"
            "#F = { q0, State_1, 123 }\n"
            "#N = 2"),
        CodeError, {
            simpletest("t23_type", e.type, CodeError::Type::VALIDATOR_MISSING_UNDERSCORE_IN_TAPE_ALPHABET);
            simpletest("t23_span", e.span, e.span.code->span(2, 5, 2, 13));
        })
    TEST_ERROR(
        "t24",
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1, \" }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 2"),
        CodeError, {
            simpletest("t24_type", e.type, CodeError::Type::VALIDATOR_INPUT_ALPHABET_NOT_SUBSET_OF_TAPE_ALPHABET);
            simpletest("t24_span", e.span, e.span.code->span(1, 5, 1, 16));
        })
    TEST_ERROR(
        "t25",
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = Q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 123 }\n"
              "#N = 2"),
        CodeError, {
            simpletest("t25_type", e.type, CodeError::Type::VALIDATOR_INVALID_STATE);
            simpletest("t25_span", e.span, e.span.code->span(3, 6, 3, 8));
            simpletest("t25_expected", e.info, INITIAL_STATE);
        })
    TEST_ERROR(
        "t26",
        parse("#Q = { q0, State_1, 123, helloworld }\n"
              "#S = { 0, 1 }\n"
              "#G = { 0, 1, _ }\n"
              "#q0 = q0\n"
              "#B = _\n"
              "#F = { q0, State_1, 321 }\n"
              "#N = 2\n"),
        CodeError, {
            simpletest("t26_type", e.type, CodeError::Type::VALIDATOR_FINAL_STATES_NOT_SUBSET_OF_STATES);
            simpletest("t26_span", e.span, e.span.code->span(5, 5, 5, 25));
        })
    TEST_ERROR("t27", parse(basic + "q0 00 01 rl"), CodeError, {
        simpletest("t27_type", e.type, CodeError::Type::PARSER_TRANSITION_TOO_FEW_ITEMS);
        simpletest("t27_span", e.span, e.span.code->span(7, 0, 7, 11));
    })
    TEST_ERROR("t28", parse(basic + "q0 00 01 rl 123 q0"), CodeError, {
        simpletest("t28_type", e.type, CodeError::Type::PARSER_TRANSITION_TOO_FEW_ITEMS);
        simpletest("t28_span", e.span, e.span.code->span(7, 16, 7, 18));
    })
    TEST_ERROR("t29", parse(basic + "q0 00 01 rl\n 123"), CodeError, {
        simpletest("t29_type", e.type, CodeError::Type::PARSER_TRANSITION_NOT_ON_SAME_LINE);
        simpletest("t29_span", e.span, e.span.code->span(7, 0, 8, 4));
    })
    TEST_ERROR("t30", parse(basic + "invalid 00 01 rl 123"), CodeError, {
        simpletest("t30_type", e.type, CodeError::Type::VALIDATOR_INVALID_STATE);
        simpletest("t30_span", e.span, e.span.code->span(7, 0, 7, 7));
    })
    TEST_ERROR("t31", parse(basic + "q0 00 01 rl invalid"), CodeError, {
        simpletest("t31_type", e.type, CodeError::Type::VALIDATOR_INVALID_STATE);
        simpletest("t31_span", e.span, e.span.code->span(7, 12, 7, 19));
    })
    TEST_ERROR("t32", parse(basic + "q0 0 01 lr 123"), CodeError, {
        simpletest("t32_type", e.type, CodeError::Type::VALIDATOR_TRASITION_ITEM_INVALID_LENGTH);
        simpletest("t32_span", e.span, e.span.code->span(7, 3, 7, 4));
        simpletest("t32_expected", e.info, "2");
    })
    TEST_ERROR("t33", parse(basic + "q0 00 01 lrl 123"), CodeError, {
        simpletest("t33_type", e.type, CodeError::Type::VALIDATOR_TRASITION_ITEM_INVALID_LENGTH);
        simpletest("t33_span", e.span, e.span.code->span(7, 9, 7, 12));
        simpletest("t33_expected", e.info, "2");
    })
    TEST_ERROR("t34", parse(basic + "q0 _* 0? rl 123"), CodeError, {
        simpletest("t34_type", e.type, CodeError::Type::VALIDATOR_NOT_IN_TAPE_SYMBOL_AND_NOT_WILDCARD);
        simpletest("t34_span", e.span, e.span.code->span(7, 7, 7, 8));
    })
    TEST_ERROR("t35", parse(basic + "q0 _* 01 *u 123"), CodeError, {
        simpletest("t35_type", e.type, CodeError::Type::VALIDATOR_INVALID_DIRECTION);
        simpletest("t35_span", e.span, e.span.code->span(7, 10, 7, 11));
    })
    basic += "q0 _* 01 rl 123\n";
    basic += "q0 0* _* lr helloworld\n";
    auto tm = parse(basic);
    simpletest("tm_q", tm->Q,
               std::set<std::string>{"q0", "State_1", "123", "helloworld"});
    simpletest("tm_S", tm->S, std::set<char>{'0', '1'});
    simpletest("tm_G", tm->G, std::set<char>{'0', '1', '_'});
    simpletest("tm_q0", tm->q0, "q0");
    simpletest("tm_B", tm->B, '_');
    simpletest("tm_F", tm->F, std::set<std::string>{"q0", "State_1", "123"});
    simpletest("tm_N", tm->N, 2u);
    simpletest("tm_delta_1", tm->delta[0], Transition{"q0", "_*", "01", "rl", "123"});
    simpletest("tm_delta_2", tm->delta[1], Transition{"q0", "0*", "_*", "lr", "helloworld"});

    return 0;
}
