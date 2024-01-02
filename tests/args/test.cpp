#include "color.h"
#include "str_literals.h"
#include "util.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace util;
using namespace str_literals;

std::filesystem::path turing = findFileWithPrefix("turing");
std::filesystem::path testdir = "../tests/args";
std::filesystem::path tempdir = std::filesystem::temp_directory_path() / "testargs";
std::filesystem::path nonexistdir = tempdir / "nonexist";
std::filesystem::path unreadabledir = tempdir / "unreadable.txt";

void test(std::string_view test_name, std::vector<std::string_view> args,
          int expected_code, std::vector<std::string_view> expected_output) {
    int actual_code = runCommand(turing, "/dev/null", tempdir / "actual.txt", args);
    TermColor::setForceColor(true);
    if (actual_code != expected_code) {
        std::cerr << FAILED << test_name << std::endl;
        std::cerr << "expected code: " << expected_code << std::endl;
        std::cerr << "  actual code: " << actual_code << std::endl;
        exit(1);
    }
    std::ifstream actual_file(tempdir / "actual.txt");
    int lineno = 0;
    for (auto &&expected_line : expected_output) {
        lineno++;
        std::string actual_line;
        std::getline(actual_file, actual_line);
        if (actual_line != expected_line) {
            std::cerr << FAILED << test_name << std::endl;
            std::cerr << "(in line " << lineno << ")" << std::endl;
            std::cerr << "expected: " << expected_line << std::endl;
            std::cerr << "  actual: " << actual_line << std::endl;
            exit(1);
        }
    }
    std::cerr << PASSED << test_name << std::endl;
    TermColor::setForceColor(false);
}

int main() {
    std::filesystem::remove_all(tempdir);
    std::filesystem::create_directories(tempdir);
    std::ofstream out(unreadabledir);
    out.close();
    chmod(unreadabledir.string().data(), 0000);

    test("help", {"-h"}, 0, {USAGE});
    test("no_tm_path", {}, 1, {ERR + TOO_FEW_ARGS, NOTE + USAGE});
    test("no_input", {"1"}, 1, {ERR + TOO_FEW_ARGS, NOTE + USAGE});
    test("too_many_args", {"1", "2", "3"}, 1, {ERR + TOO_MANY_ARGS, NOTE + USAGE});
    test("invalid_option", {"-x"}, 1, {ERR + format(INVALID_OPTION, "-x"), NOTE + USAGE});
    test("empty_tm_path", {"", ""}, 1,
         {ERR + format(FILE_NOT_EXIST, std::filesystem::path{})});
    test("read_dir", {tempdir.string(), "input"}, 1,
         {ERR + format(NOT_A_REGULAR_FILE, tempdir)});
    test("read_nonexist", {(tempdir / "nonexist").string(), "input"}, 1,
         {ERR + format(FILE_NOT_EXIST, tempdir / "nonexist")});
    test("unreadable", {(tempdir / "unreadable.txt").string(), "input"}, 1,
         {ERR + format(UNABLE_TO_READ, tempdir / "unreadable.txt")});

    return 0;
}
