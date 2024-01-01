#include "code.h"
#include "util.hpp"
#include <filesystem>

using namespace util;

int main() {
    TermColor::setForceColor();

    std::filesystem::path base = "../tests/code";
    auto test_file = std::make_shared<Code>(base / "test.txt");

    simpletest("path", test_file->path(), base / "test.txt");
    simpletest("lines", test_file->lines(), 4);
    simpletest("line", test_file->line(0), "Hello!");
    simpletest("line2", test_file->line(3), "");
    simpletest("line_len", test_file->lineLen(0), 6);
    simpletest("line_len2", test_file->lineLen(3), 0);
    simpletest("char_at_begin", test_file->charAt(test_file->begin()), 'H');
    simpletest("char_at_begin_of_line", test_file->charAt(test_file->begin(0)), 'H');
    simpletest("char_at_end", test_file->charAt(test_file->end()), '\0');
    simpletest("char_at_end_of_line", test_file->charAt(test_file->end(0)), '\n');

    auto it = test_file->begin();
    simpletest("cursor_bof_1", it.bof(), true);
    simpletest("cursor_eof_1", it.eof(), false);
    simpletest("cursor_inc", ++it, Code::Cursor{test_file, 0, 1});
    simpletest("cursor_inc_post", it++, Code::Cursor{test_file, 0, 1});
    simpletest("cursor_inc_post_2", it, Code::Cursor{test_file, 0, 2});
    simpletest("cursor_skip_line", it.skipLine(), Code::Cursor{test_file, 1, 0});
    simpletest("cursor_dec", --it, Code::Cursor{test_file, 0, 6});
    auto it2 = Code::Cursor{test_file, 1, 10};
    simpletest("cursor_neq", it != it2, true);
    simpletest("cursor_skip_prev_line", it2.skipPrevLine(),
               Code::Cursor{test_file, 0, 6});
    simpletest("cursor_eq", it == it2, true);
    auto it3 = test_file->end();
    simpletest("cursor_bof_2", it3.bof(), false);
    simpletest("cursor_eof_2", it3.eof(), true);
    simpletest("cursor_dec_post", it3--, Code::Cursor{test_file, 4, 0});
    simpletest("cursor_dec_post_2", it3, Code::Cursor{test_file, 3, 0});
    simpletest("cursor_deref", *it3, '\n');

    auto it4 = test_file->begin(0);
    auto it5 = test_file->end(0);
    auto sp1 = test_file->span(it4, it5);
    simpletest("span_1", sp1, Code::Span{test_file, 0, 0, 0, 6});
    simpletest("span_empty_1", sp1.empty(), false);
    simpletest("span_front", sp1.front(), 'H');
    simpletest("span_back", sp1.back(), '!');
    sp1.extend();
    simpletest("span_extend", sp1, Code::Span{test_file, 0, 0, 1, 0});
    simpletest("span_str_1", sp1.str(), "Hello!\n");
    sp1.extendLine();
    simpletest("span_extend_line", sp1, Code::Span{test_file, 0, 0, 2, 0});
    simpletest("span_begin", sp1.begin(), Code::Cursor{test_file, 0, 0});
    simpletest("span_end", sp1.end(), Code::Cursor{test_file, 2, 0});
    auto sp2 = test_file->span(0, 0, 2, 0);
    simpletest("span_2", sp2, Code::Span{test_file, 0, 0, 2, 0});
    simpletest("span_eq", sp1 == sp2, true);
    sp1.collapse();
    simpletest("span_collapse", sp1, Code::Span{test_file, 2, 0, 2, 0});
    simpletest("span_empty_2", sp1.empty(), true);
    simpletest("span_str_2", sp1.str(), "");
    auto sp3 = test_file->span(sp2, sp1);
    simpletest("span_3", sp3, Code::Span{test_file, 0, 0, 2, 0});
    sp3.extendLine();
    sp3.collapse();
    simpletest("span_eof_1", sp3.eof(), false);
    sp3.extendLine();
    simpletest("span_eof_2", sp3.eof(), false);
    sp3.collapse();
    simpletest("span_eof_3", sp3.eof(), true);

    std::stringstream ss;
    test_file->printHighlight(test_file->span(0, 0, 0, 0), RED, ss);
    simpletest("print_highlight_1", ss.str(),
               "   1 | " + Painted("H", RED) + "ello!\n"
               "     | " + Painted("~", RED) + "     \n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 0, 0, 5), RED, ss);
    simpletest("print_highlight_2", ss.str(),
               "   1 | " + Painted("Hello", RED) + "!\n"
               "     | " + Painted("~~~~~", RED) + " \n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 0, 0, 6), RED, ss);
    simpletest("print_highlight_3", ss.str(),
               "   1 | " + Painted("Hello!", RED) + "\n"
               "     | " + Painted("~~~~~~", RED) + "\n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 5, 1, 0), RED, ss);
    simpletest("print_highlight_4", ss.str(),
               "   1 | Hello" + Painted("!", RED) + "\n"
               "     |      " + Painted("~", RED) + "\n"
               "   2 | " + Painted("", RED) + "\n"
               "     | " + Painted("", RED) + "\n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 0, 2, 0), RED, ss);
    simpletest("print_highlight_5", ss.str(),
               "   1 | " + Painted("Hello!", RED) + "\n"
               "     | " + Painted("~~~~~~", RED) + "\n"
               "   2 | " + Painted("", RED) + "\n"
               "     | " + Painted("", RED) + "\n"
               "   3 | " + Painted("T", RED) + "his is a test file with 4 lines, but the 2nd and 4th lines are empty." + "\n"
               "     | " + Painted("~", RED) + "                                                                     " + "\n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 0, 2, 1), RED, ss);
    simpletest("print_highlight_6", ss.str(),
               "   1 | " + Painted("Hello!", RED) + "\n"
               "     | " + Painted("~~~~~~", RED) + "\n"
               "   2 | " + Painted("", RED) + "\n"
               "     | " + Painted("", RED) + "\n"
               "   3 | " + Painted("T", RED) + "his is a test file with 4 lines, but the 2nd and 4th lines are empty." + "\n"
               "     | " + Painted("~", RED) + "                                                                     " + "\n");
    ss.str("");
    test_file->printHighlight(test_file->span(0, 5, 2, 4), RED, ss);
    simpletest("print_highlight_7", ss.str(),
               "   1 | Hello" + Painted("!", RED) + "\n"
               "     |      " + Painted("~", RED) + "\n"
               "   2 | " + Painted("", RED) + "\n"
               "     | " + Painted("", RED) + "\n"
               "   3 | " + Painted("This", RED) + " is a test file with 4 lines, but the 2nd and 4th lines are empty.\n"
               "     | " + Painted("~~~~", RED) + "                                                                  \n");
    ss.str("");
    test_file->printHighlight(test_file->span(3, 0, 3, 0), RED, ss);
    simpletest("print_highlight_8", ss.str(),
               "   4 | " + Painted("", RED) + "\n"
               "     | " + Painted("", RED) + "\n");
    ss.str("");
    test_file->printHighlight(test_file->span(4, 0, 4, 0), RED, ss);
    simpletest("print_highlight_9", ss.str(),
               "   5 | " + Painted(" ", RED) + "\n"
               "     | " + Painted("~", RED) + "\n");
    return 0;
}
