#include "color.h"
#include "util.hpp"

int main() {
    using namespace util;
    TermColor::setForceColor();
    simpletest("quoted_newline", quoted('\n'), "'\\n'");
    simpletest("quoted_tab", quoted('\t'), "'\\t'");
    simpletest("quoted_carriage_return", quoted('\r'), "'\\r'");
    simpletest("quoted_null", quoted('\0'), "'\\0'");
    simpletest("quoted_single_quote", quoted('\''), "'\\''");
    simpletest("quoted_backslash", quoted('\\'), "'\\\\'");
    simpletest("quoted_backspace", quoted('\b'), "'\\b'");
    simpletest("quoted_form_feed", quoted('\f'), "'\\f'");
    simpletest("quoted_vertical_tab", quoted('\v'), "'\\v'");
    simpletest("quoted_alert", quoted('\a'), "'\\a'");
    simpletest("quoted_printable", quoted('a'), "'a'");
    simpletest("quoted_small_nonprintable", quoted('\x01'), "'\\x01'");
    simpletest("quoted_large_nonprintable", quoted('\xff'), "'\\xFF'");

    simpletest("quoted_string_empty", quoted(""), "\"\"");
    simpletest("quoted_string_nonempty", quoted("hello"), "\"hello\"");
    simpletest("quoted_string_with_escape", quoted("hello\nworld"), "\"hello\\nworld\"");
    simpletest("quoted_string_with_nonprintable", quoted("\xBEST"), "\"\\xBEST\"");

    simpletest("format_empty", format(""), "");
    simpletest("format_no_args", format("hello"), "hello");
    simpletest("format_one_arg", format("hello {}", "world"), "hello world");
    simpletest("format_two_args", format("hello {} {}", "world", "again"),
               "hello world again");
    simpletest("format_escape", format("hello {{}}"), "hello {}");
    simpletest("format_escape_with_arg", format("hello {{}} {}", "world"),
               "hello {} world");
    simpletest("format_nested", format("hello {{{}}}", "world"), "hello {world}");
    simpletest("format_int", format("hello {}", 123), "hello 123");
    simpletest("format_char", format("hello {}", 'a'), "hello a");
    return 0;
}
