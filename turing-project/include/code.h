#ifndef __CODE_H__
#define __CODE_H__

#include <string>
#include <string_view>
#include <vector>

class Code {
  private:
    std::string path_;
    std::vector<std::string> lines_;

  public:
    struct Cursor;
    struct InlineCursor;
    Code(std::string_view path);
    std::string_view path() const;
    int lines() const;
    std::string_view line(int lno) const;
    int lineLen(int lno) const;
    Cursor begin() const;
    InlineCursor begin(int lno) const;
    Cursor end() const;
    InlineCursor end(int lno) const;
    char charAt(const Cursor &cs) const;
    void printLine(int lno) const;
    void printLines(int st_lno, int ed_lno) const;
    void printHighlight(InlineCursor st, InlineCursor ed, const char *color) const;
    void printHighlights(Cursor st, Cursor ed, const char *color) const;
};

struct Code::Cursor {
    const Code *code;
    int lno;
    int cno;

    Cursor(const Code *code, int lno, int cno);
    Cursor(InlineCursor ic);
    Cursor &operator++();
    Cursor operator++(int);
    Cursor &operator--();
    Cursor operator--(int);
    char operator*() const;
    bool operator!=(const Cursor &other) const;
    Cursor &skipLine();
    Cursor &skipPrevLine();
    bool bof() const;
    bool eof() const;
};

struct Code::InlineCursor {
    const Code *code;
    int lno;
    int cno;

    InlineCursor(const Code *code, int lno, int cno);
    InlineCursor(Cursor cs);
    InlineCursor &operator++();
    InlineCursor operator++(int);
    InlineCursor &operator--();
    InlineCursor operator--(int);
    char operator*() const;
    bool operator!=(const InlineCursor &other) const;
    bool eol() const;
};

#endif
