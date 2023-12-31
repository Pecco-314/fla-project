#include "color.h"
#include "messages.h"
#include "util.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace util;
using namespace msg;

std::filesystem::path findTuring() {
    std::filesystem::path cur = std::filesystem::current_path();
    for (auto &&p : std::filesystem::directory_iterator(cur)) {
        if (p.is_regular_file() && p.path().filename().string().find("turing") == 0) {
            return p.path();
        }
    }
    std::cerr << ERR << "turing executable file not found" << std::endl;
    exit(1);
}

std::filesystem::path turing = findTuring();
std::filesystem::path testdir = "../tests/args";
std::filesystem::path tempdir = "temp";
std::filesystem::path nonexistdir = tempdir / "nonexist";
std::filesystem::path unreadabledir = tempdir / "unreadable.txt";

std::string formatCommand(const std::vector<std::string_view> &args) {
    std::string command = "sh -c '" + turing.string();
    for (auto &&arg : args) {
        command += " " + quoted(arg);
    }
    command += " 1>/dev/null 2>";
    command += tempdir / "actual.txt";
    command += "'";
    return command;
}

void test(std::string_view test_name, std::vector<std::string_view> args,
          int expected_code, std::vector<std::string_view> expected_output) {
    std::string cmd = formatCommand(args);
    int actual_code = WEXITSTATUS(system(cmd.data()));
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
