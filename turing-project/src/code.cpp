#include "code.h"
#include <fstream>
#include <iostream>

Code::Code(std::filesystem::path path) : path_(path) {
    if (!std::filesystem::exists(path_)) {
        std::cerr << ERR << path << " does not exist" << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_regular_file(path_)) {
        std::cerr << ERR << path << " is not a regular file" << std::endl;
        exit(1);
    }
    std::ifstream tmfile(path_);
    if (!tmfile.good()) {
        std::cerr << ERR << "Unable to read " << path << std::endl;
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
    return lines_[lno];
}

int Code::lineLen(int lno) const {
    return lines_[lno].size();
}

char Code::charAt(const Cursor &cs) const {
    if (cs.lno >= lines()) {
        return '\0';
    } else if (cs.cno == lineLen(cs.lno)) {
        return '\n';
    } else {
        return lines_[cs.lno][cs.cno];
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

void Code::printLineHighlight(Span span, TermColor color) const {
    int lno = span.st_lno;
    if (span.eof()) {
        std::cerr << std::setw(4) << lno + 1 << " |" << std::endl;
        std::cerr << "     | " << color << "~" << RESET << std::endl;
        return;
    }
    if (span.empty()) { span.extend(); }
    std::string_view bf = line(lno).substr(0, span.st_cno);
    std::string_view hl = line(lno).substr(span.st_cno, span.ed_cno - span.st_cno);
    std::string_view af = line(lno).substr(span.ed_cno);
    std::cerr << std::setw(4) << lno + 1 << " | " << bf << color << hl << RESET << af
              << std::endl;
    std::cerr << "     | " << std::string(bf.size(), ' ') << color
              << std::string(hl.size(), '~') << RESET << std::string(af.size(), ' ')
              << std::endl;
}

void Code::printHighlight(Span span, TermColor color) const {
    auto &&code = span.code;
    for (int lno = span.st_lno; lno <= span.ed_lno; lno++) {
        Cursor st_cs = (lno == span.st_lno) ? span.begin() : begin(lno);
        Cursor ed_cs = (lno == span.ed_lno) ? span.end() : end(lno);
        printLineHighlight(code->span(st_cs, ed_cs), color);
    }
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

Code::Span::Span(std::shared_ptr<const Code> code, int st_lno, int st_cno, int ed_lno,
                 int ed_cno)
    : code(code), st_lno(st_lno), st_cno(st_cno), ed_lno(ed_lno), ed_cno(ed_cno) {}

Code::Span::Span(std::shared_ptr<const Code> code, const Cursor &st, const Cursor &ed)
    : Span(code, st.lno, st.cno, ed.lno, ed.cno) {}

bool Code::Span::operator==(const Span &other) const {
    return st_lno == other.st_lno && st_cno == other.st_cno && ed_lno == other.ed_lno &&
           ed_cno == other.ed_cno;
}

Code::Cursor Code::Span::begin() const {
    return {code, st_lno, st_cno};
}

Code::Cursor Code::Span::end() const {
    return {code, ed_lno, ed_cno};
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