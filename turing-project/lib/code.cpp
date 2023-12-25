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
    if (cs.cno == lineLen(cs.lno)) {
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

void Code::printLineHighlight(Cursor st, Cursor ed, const char *color) const {
    int lno = st.lno;
    if (st == ed) { ++ed; }
    std::string_view bf = line(lno).substr(0, st.cno);
    std::string_view hl = line(lno).substr(st.cno, ed.cno - st.cno);
    std::string_view af = line(lno).substr(ed.cno);
    std::cerr << std::setw(4) << lno + 1 << " | " << bf << color << hl << RESET << af
              << std::endl;
    std::cerr << "     | " << std::string(bf.size(), ' ') << color << '^'
              << std::string(hl.size() - 1, '~') << RESET << std::string(af.size(), ' ')
              << std::endl;
}

void Code::printHighlight(Cursor st, Cursor ed, const char *color) const {
    for (int lno = st.lno; lno <= ed.lno; lno++) {
        Cursor st_ic = (lno == st.lno) ? st : begin(lno);
        Cursor ed_ic = (lno == ed.lno) ? ed : end(lno);
        printLineHighlight(st_ic, ed_ic, color);
    }
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

std::string Code::Cursor::span(const Cursor &ed) const {
    std::string ret;
    for (Cursor cs = *this; cs != ed; ++cs) {
        ret += *cs;
    }
    return ret;
}
