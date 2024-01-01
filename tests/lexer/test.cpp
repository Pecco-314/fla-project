#include "lexer.h"
#include "error.h"
#include "util.hpp"
#include <fstream>

using namespace util;

std::vector<Token> lex(std::string_view text) {
    std::ofstream file("test.tm");
    file << text;
    file.close();
    auto code = std::make_shared<Code>("test.tm");
    Lexer lexer(code);
    return lexer.lex();
}

void test1() {
    auto tokens = lex("\t");
    simpletest("t1_size", tokens.size(), 0ull);
}

void test2() {
    auto tokens = lex("#Q = { ab, 123, A, hello_world, hello}");
    simpletest("t2_size", tokens.size(), 13ull);
    simpletest("t2_0", tokens[0].val, "#Q");
    simpletest("t2_1", tokens[1].val, "=");
    simpletest("t2_2", tokens[2].val, "{");
    simpletest("t2_3", tokens[3].val, "ab");
    simpletest("t2_4", tokens[4].val, ",");
    simpletest("t2_5", tokens[5].val, "123");
    simpletest("t2_6", tokens[6].val, ",");
    simpletest("t2_7", tokens[7].val, "A");
    simpletest("t2_8", tokens[8].val, ",");
    simpletest("t2_9", tokens[9].val, "hello_world");
    simpletest("t2_10", tokens[10].val, ",");
    simpletest("t2_11", tokens[11].val, "hello");
    simpletest("t2_12", tokens[12].val, "}");
}

void test3() {
    auto tokens = lex("#G = { 1, _, ?, \", # }");
    simpletest("t3_size", tokens.size(), 13ull);
    simpletest("t3_0", tokens[0].val, "#G");
    simpletest("t3_1", tokens[1].val, "=");
    simpletest("t3_2", tokens[2].val, "{");
    simpletest("t3_3", tokens[3].val, "1");
    simpletest("t3_4", tokens[4].val, ",");
    simpletest("t3_5", tokens[5].val, "_");
    simpletest("t3_6", tokens[6].val, ",");
    simpletest("t3_7", tokens[7].val, "?");
    simpletest("t3_8", tokens[8].val, ",");
    simpletest("t3_9", tokens[9].val, "\"");
    simpletest("t3_10", tokens[10].val, ",");
    simpletest("t3_11", tokens[11].val, "#");
    simpletest("t3_12", tokens[12].val, "}");
}

void test4() {
    auto tokens = lex("a#b-c ; comments\n\t d; comments");
    simpletest("t4_size", tokens.size(), 5ull);
    simpletest("t4_0", tokens[0].val, "a");
    simpletest("t4_1", tokens[1].val, "#b");
    simpletest("t4_2", tokens[2].val, "-");
    simpletest("t4_3", tokens[3].val, "c");
    simpletest("t4_4", tokens[4].val, "d");
}

void test5() {
    try {
        auto tokens = lex("2024年");
    } catch (CodeError &e) {
        auto code = e.span.code;
        simpletest("t5", e.type, CodeError::Type::LEXER_INVALID_CHAR);
        simpletest("t5_span", e.span, code->span(0, 4, 0, 4));
    }
}

int main() {
    TermColor::setForceColor();
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}
