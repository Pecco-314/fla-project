#include "code.h"
#include "constants.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Code::Code(std::string_view path) : path_(path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << ERR << UL << path << RESET << " does not exist" << std::endl;
        exit(1);
    }
    if (!std::filesystem::is_regular_file(path)) {
        std::cerr << ERR << UL << path << RESET << " is not a regular file" << std::endl;
        exit(1);
    }
    std::ifstream tmfile(path.data());
    if (!tmfile.good()) {
        std::cerr << ERR << "Unable to read " << UL << path << RESET << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(tmfile, line)) {
        lines_.push_back(line);
    }
}

std::string_view Code::path() const {
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
    return lines_[cs.lno][cs.cno];
}

Code::Cursor Code::begin() const {
    Cursor bg = {this, 0, 0};
    if (lineLen(0) == 0) {
        return bg.skipLine();
    } else {
        return bg;
    }
}

Code::InlineCursor Code::begin(int lno) const {
    return {this, lno, 0};
}

Code::Cursor Code::end() const {
    return {this, lines(), 0};
}

Code::InlineCursor Code::end(int lno) const {
    return {this, lno, lineLen(lno)};
}

void Code::printLine(int lno) const {
    std::cerr << std::setw(4) << lno + 1 << " | " << line(lno) << std::endl;
}

void Code::printLines(int st_lno, int ed_lno) const {
    for (int lno = st_lno; lno <= ed_lno; lno++) {
        printLine(lno);
    }
}

void Code::printHighlight(InlineCursor st, InlineCursor ed, const char *color) const {
    int lno = st.lno;
    auto ic = begin(lno);
    std::cerr << "     | ";
    for (; ic != st; ++ic) {
        std::cerr << ' ';
    }
    std::cerr << color;
    for (; ic != ed; ++ic) {
        std::cerr << '~';
    }
    std::cerr << RESET;
    for (; ic != end(lno); ++ic) {
        std::cerr << ' ';
    }
    std::cerr << std::endl;
}

void Code::printHighlights(Cursor st, Cursor ed, const char *color) const {
    InlineCursor ist = st;
    InlineCursor ied = ed;
    if (ed.cno == 0) {
        ied = ++InlineCursor(++ed);
    }
    for (int lno = ist.lno; lno <= ied.lno; lno++) {
        InlineCursor st_ic = (lno == ist.lno) ? ist : begin(lno);
        InlineCursor ed_ic = (lno == ied.lno) ? ied : end(lno);
        printLine(lno);
        printHighlight(st_ic, ed_ic, color);
    }
}

Code::Cursor::Cursor(const Code *code, int lno, int cno) : code(code), lno(lno), cno(cno) {}

Code::Cursor::Cursor(InlineCursor ic) : code(ic.code), lno(ic.lno), cno(ic.cno) {
    if (ic.eol()) {
        skipLine();
    }
}

Code::Cursor &Code::Cursor::operator++() {
    cno++;
    if (cno >= code->lineLen(lno)) {
        skipLine();
    }
    return *this;
}

Code::Cursor Code::Cursor::operator++(int) {
    Cursor tmp = *this;
    ++*this;
    return tmp;
}

Code::Cursor &Code::Cursor::operator--() {
    cno--;
    if (cno < 0) {
        skipPrevLine();
    }
    return *this;
}

char Code::Cursor::operator*() const {
    return code->charAt(*this);
}

bool Code::Cursor::operator!=(const Cursor &other) const {
    return lno != other.lno || cno != other.cno;
}

Code::Cursor &Code::Cursor::skipLine() {
    for (lno++, cno = 0; !eof() && code->lineLen(lno) == 0; lno++)
        ;
    return *this;
}

Code::Cursor &Code::Cursor::skipPrevLine() {
    for (lno--; !bof() && code->lineLen(lno) == 0; lno--)
        ;
    if (!bof()) {
        cno = code->lineLen(lno) - 1;
    } else {
        cno = 0;
    }
    return *this;
}

bool Code::Cursor::bof() const {
    return lno < 0;
}

bool Code::Cursor::eof() const {
    return lno >= code->lines();
}

Code::InlineCursor::InlineCursor(const Code *code, int lno, int cno) : code(code), lno(lno), cno(cno) {}

Code::InlineCursor::InlineCursor(Cursor cs) : code(cs.code), lno(cs.lno), cno(cs.cno) {}

Code::InlineCursor &Code::InlineCursor::operator++() {
    cno++;
    return *this;
}

Code::InlineCursor Code::InlineCursor::operator++(int) {
    InlineCursor tmp = *this;
    ++*this;
    return tmp;
}

Code::InlineCursor &Code::InlineCursor::operator--() {
    cno--;
    return *this;
}

Code::InlineCursor Code::InlineCursor::operator--(int) {
    InlineCursor tmp = *this;
    --*this;
    return tmp;
}

char Code::InlineCursor::operator*() const {
    return code->charAt(*this);
}

bool Code::InlineCursor::operator!=(const InlineCursor &other) const {
    return lno != other.lno || cno != other.cno;
}

bool Code::InlineCursor::eol() const {
    return cno >= code->lineLen(lno);
}
