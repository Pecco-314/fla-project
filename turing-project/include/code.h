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
    Code(std::string_view path);
    std::string_view path() const;
    int lines() const;
    std::string_view line(int lno) const;
    int lineLen(int lno) const;
    Cursor begin() const;
    Cursor begin(int lno) const;
    Cursor end() const;
    Cursor end(int lno) const;
    char charAt(const Cursor &cs) const;
    void printLineHighlight(Cursor st, Cursor ed, const char *color) const;
    void printHighlight(Cursor st, Cursor ed, const char *color) const;
};

struct Code::Cursor {
    const Code *code;
    int lno;
    int cno;

    Cursor(const Code *code, int lno, int cno);
    Cursor();
    Cursor &operator++();
    Cursor operator++(int);
    Cursor &operator--();
    Cursor operator--(int);
    char operator*() const;
    bool operator!=(const Cursor &other) const;
    bool operator==(const Cursor &other) const;
    Cursor &skipLine();
    Cursor &skipPrevLine();
    std::string span(const Cursor &ed) const;
    bool bof() const;
    bool eof() const;
};

#endif
