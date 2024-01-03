#include "color.h"
#include "util.h"
#include <filesystem>
#include <fstream>
#include <iostream>

std::filesystem::path test_dir = "../tests/run";
std::filesystem::path tm_dir = test_dir / "tm";
std::filesystem::path out_dir = test_dir / "out";
std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
std::filesystem::path turing = util::findFileWithPrefix("turing");

void test(std::string_view name, std::string_view input, bool verbose = false,
          int expected_code = 0) {
    std::string testname =
        std::string(name) + "_" + std::string(input) + (verbose ? "_verbose" : "");
    auto tm_file = tm_dir / name += ".tm";
    auto expected_file = out_dir / testname += ".txt";
    auto actual_file = temp_dir / "actual.txt";
    auto debug_file = temp_dir / "debug.txt";
    auto out = expected_code ? debug_file : actual_file;
    auto err = expected_code ? actual_file : debug_file;
    std::vector<std::string> args;
    args.emplace_back(tm_file);
    args.emplace_back(input);
    if (verbose) { args.push_back("-v"); }
    auto code = util::runCommand(turing, out, err, args);
    if (expected_code != code) {
        std::cerr << FAILED << testname << std::endl;
        std::cerr << "expected code: " << expected_code << std::endl;
        std::cerr << "  actual code: " << code << std::endl;
        std::cerr << (expected_code ? "stderr" : "stdout") << ":" << std::endl;
        util::runCommand("cat", "", "", {debug_file.string()});
        exit(1);
    }

    std::ifstream expected(expected_file);
    std::ifstream actual(actual_file);
    for (int i = 0;; i++) {
        std::string expected_line, actual_line;
        std::getline(expected, expected_line);
        std::getline(actual, actual_line);
        if (!expected && !actual) { break; }
        if (!expected) {
            expected_line = "<EOF>";
        } else {
            expected_line = util::strip(expected_line);
        }
        if (!actual) {
            actual_line = "<EOF>";
        } else {
            actual_line = util::strip(actual_line);
        }
        if (expected_line != actual_line) {
            std::cerr << FAILED << testname << std::endl;
            std::cerr << "(in line " << i + 1 << ")" << std::endl;
            std::cerr << "expected: " << expected_line << std::endl;
            std::cerr << "  actual: " << actual_line << std::endl;
            std::cerr << (expected_code ? "stderr" : "stdout") << ":" << std::endl;
            util::runCommand("cat", "", "", {debug_file.string()});
            exit(1);
        }
    }
    std::cerr << PASSED << testname << std::endl;
}

int main() {
    TermColor::setForceColor();
    test("palindrome_detector_2tapes", "1001001", true);
    test("palindrome_detector_2tapes", "100A1A001", true, 1);
    return 0;
}
