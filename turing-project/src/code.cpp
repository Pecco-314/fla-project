#include "code.h"
#include "str_literals.h"
#include "util.h"
#include <fstream>
#include <iostream>

using namespace util;
using namespace str_literals;

bool Code::allow_warnings_ = false;
int Code::count_warnings_ = 0;

Code::Code(std::filesystem::path path) : path_(path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << ERR << format(FILE_NOT_EXIST, path) << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_regular_file(path)) {
        std::cerr << ERR << format(NOT_A_REGULAR_FILE, path) << std::endl;
        exit(1);
    }
    std::ifstream tmfile(path);
    if (!tmfile.good()) {
        std::cerr << ERR << format(UNABLE_TO_READ, path) << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(tmfile, line)) {
        lines_.push_back(line);
    }
}

std::filesystem::path Code::path() const {
    return path_;
}

int Code::lines() const {
    return lines_.size();
}

std::string_view Code::line(int lno) const {
    return lines_.at(lno);
}

int Code::lineLen(int lno) const {
    return lines_.at(lno).size();
}

char Code::charAt(const Cursor &cs) const {
    if (cs.lno >= lines()) {
        return '\0';
    } else if (cs.cno == lineLen(cs.lno)) {
        return '\n';
    } else {
        return lines_.at(cs.lno).at(cs.cno);
    }
}

void Code::setAllowWarnings(bool print_warnings) {
    allow_warnings_ = print_warnings;
}

void Code::printWarningCount() {
    if (count_warnings_ > 0) {
        std::cerr << YELLOW << count_warnings_ << " warnings generated" << RESET << std::endl;
    }
}

Code::Cursor Code::begin() const {
    return {shared_from_this(), 0, 0};
}

Code::Cursor Code::begin(int lno) const {
    return {shared_from_this(), lno, 0};
}

Code::Cursor Code::end() const {
    return {shared_from_this(), lines(), 0};
}

Code::Cursor Code::end(int lno) const {
    return {shared_from_this(), lno, lineLen(lno)};
}

void Code::printLineHighlight(Span span, TermColor color, std::ostream &os) const {
    int lno = span.st_lno;
    if (span.eof()) {
        os << std::setw(4) << lno + 1 << " | " << Painted(" ", color) << std::endl;
        os << "     | " << Painted("~", color) << std::endl;
        return;
    }
    if (span.empty()) { span.extend(); }
    std::string_view bf = line(lno).substr(0, span.st_cno);
    std::string_view hl = line(lno).substr(span.st_cno, span.ed_cno - span.st_cno);
    std::string_view af = line(lno).substr(span.ed_cno);
    os << std::setw(4) << lno + 1 << " | " << bf << Painted(hl, color) << af << std::endl;
    os << "     | " << std::string(bf.size(), ' ')
       << Painted(std::string(hl.size(), '~'), color) << std::string(af.size(), ' ')
       << std::endl;
}

void Code::printHighlight(Span span, TermColor color, std::ostream &os) const {
    auto &&code = span.code;
    for (int lno = span.st_lno; lno <= span.ed_lno; lno++) {
        Cursor st_cs = (lno == span.st_lno) ? span.begin() : begin(lno);
        Cursor ed_cs = (lno == span.ed_lno) ? span.end() : end(lno);
        printLineHighlight(code->span(st_cs, ed_cs), color, os);
    }
}

void Code::printMessages(int level, std::string_view msg, const Span &span) const {
    auto &&head = level == 0 ? ERR : level == 1 ? WARN : NOTE;
    auto &&color = level == 0 ? RED | BOLD : level == 1 ? YELLOW | BOLD : BLUE | BOLD;
    std::cerr << BOLD << span.begin() << ": " << head << msg << RESET << std::endl;
    span.code->printHighlight(span, color);
}

void Code::warning(std::string_view msg, const Span &span) const {
    if (!allow_warnings_) { return; }
    printMessages(1, msg, span);
    count_warnings_++;
}

Code::Span Code::span(const Cursor &st, const Cursor &ed) const {
    return {shared_from_this(), st.lno, st.cno, ed.lno, ed.cno};
}

Code::Span Code::span(int st_lno, int st_cno, int ed_lno, int ed_cno) const {
    return {shared_from_this(), st_lno, st_cno, ed_lno, ed_cno};
}

Code::Span Code::span(const Span &st, const Span &ed) const {
    return {shared_from_this(), st.st_lno, st.st_cno, ed.ed_lno, ed.ed_cno};
}

Code::Cursor::Cursor(std::shared_ptr<const Code> code, int lno, int cno)
    : code(code), lno(lno), cno(cno) {}

Code::Cursor &Code::Cursor::operator++() {
    if (eof()) { return *this; }
    cno++;
    if (cno > code->lineLen(lno)) { skipLine(); }
    return *this;
}

Code::Cursor Code::Cursor::operator++(int) {
    Cursor tmp = *this;
    ++*this;
    return tmp;
}

Code::Cursor &Code::Cursor::operator--() {
    if (bof()) { return *this; }
    cno--;
    if (cno < 0) { skipPrevLine(); }
    return *this;
}

Code::Cursor Code::Cursor::operator--(int) {
    Cursor tmp = *this;
    --*this;
    return tmp;
}

char Code::Cursor::operator*() const {
    return code->charAt(*this);
}

bool Code::Cursor::operator!=(const Cursor &other) const {
    return lno != other.lno || cno != other.cno;
}

bool Code::Cursor::operator==(const Cursor &other) const {
    return lno == other.lno && cno == other.cno;
}

Code::Cursor &Code::Cursor::skipLine() {
    lno++;
    cno = 0;
    return *this;
}

Code::Cursor &Code::Cursor::skipPrevLine() {
    lno--;
    cno = code->lineLen(lno);
    return *this;
}

bool Code::Cursor::bof() const {
    return lno == 0 && cno == 0;
}

bool Code::Cursor::eof() const {
    return lno >= code->lines();
}

Code::Span Code::Cursor::span() const {
    auto it = *this;
    return code->span(*this, ++it);
}

Code::Span Code::Cursor::span(const Cursor &other) const {
    return code->span(*this, other);
}

std::ostream &operator<<(std::ostream &os, const Code::Cursor &cs) {
    return os << cs.code->path() << ":" << cs.lno + 1 << ":" << cs.cno + 1;
}

Code::Span::Span(std::shared_ptr<const Code> code, int st_lno, int st_cno, int ed_lno,
                 int ed_cno)
    : code(code), st_lno(st_lno), st_cno(st_cno), ed_lno(ed_lno), ed_cno(ed_cno) {}

Code::Span::Span(std::shared_ptr<const Code> code, const Cursor &st, const Cursor &ed)
    : Span(code, st.lno, st.cno, ed.lno, ed.cno) {}

bool Code::Span::operator==(const Span &other) const {
    return st_lno == other.st_lno && st_cno == other.st_cno && ed_lno == other.ed_lno &&
           ed_cno == other.ed_cno;
}

bool Code::Span::operator!=(const Span &other) const {
    return !(*this == other);
}

Code::Cursor Code::Span::begin() const {
    return {code, st_lno, st_cno};
}

Code::Cursor Code::Span::end() const {
    return {code, ed_lno, ed_cno};
}

char Code::Span::front() const {
    return code->charAt(begin());
}

char Code::Span::back() const {
    return code->charAt(--end());
}

std::string Code::Span::str() const {
    std::string res;
    for (Cursor cs = begin(); cs != end(); ++cs) {
        res += *cs;
    }
    return res;
}

void Code::Span::extend() {
    auto cs = ++end();
    ed_lno = cs.lno;
    ed_cno = cs.cno;
}

void Code::Span::extendLine() {
    auto cs = end().skipLine();
    ed_lno = cs.lno;
    ed_cno = cs.cno;
}

void Code::Span::collapse() {
    st_lno = ed_lno;
    st_cno = ed_cno;
}

bool Code::Span::empty() const {
    return st_lno == ed_lno && st_cno == ed_cno;
}

bool Code::Span::eof() const {
    return st_lno >= code->lines();
}

bool Code::Span::nextTo(const Span &other) const {
    return ed_lno == other.st_lno && ed_cno == other.st_cno;
}

std::ostream &operator<<(std::ostream &os, const Code::Span &span) {
    return os << span.code->path() << ":" << span.st_lno + 1 << ":" << span.st_cno + 1
              << "-" << span.ed_lno + 1 << ":" << span.ed_cno + 1;
}
