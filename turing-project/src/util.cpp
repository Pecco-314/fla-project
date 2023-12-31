#include "util.h"

std::string util::quoted(char ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == '\'') {
        return "'\\''";
    } else if (ch == '\\') {
        return "'\\\\'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\v') {
        return "'\\v'";
    } else if (ch == '\a') {
        return "'\\a'";
    } else if (isprint(ch)) {
        return {'\'', ch, '\''};
    } else {
        static const char HEX[] = "0123456789ABCDEF";
        unsigned char c = ch;
        return {'\'', '\\', 'x', HEX[c / 16], HEX[c % 16], '\''};
    }
}
