#ifndef __CODE_H__
#define __CODE_H__

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <iostream>
#include "color.h"

class Code : public std::enable_shared_from_this<Code> {
  private:
    std::filesystem::path path_;
    std::vector<std::string> lines_;

  public:
    struct Cursor;
    struct Span;
    Code(std::filesystem::path path);
    std::filesystem::path path() const;
    int lines() const;
    std::string_view line(int lno) const;
    int lineLen(int lno) const;
    Cursor begin() const;
    Cursor begin(int lno) const;
    Cursor end() const;
    Cursor end(int lno) const;
    Span span(const Cursor &st, const Cursor &ed) const;
    Span span(int st_lno, int st_cno, int ed_lno, int ed_cno) const;
    Span span(const Span &st, const Span &ed) const;
    char charAt(const Cursor &cs) const;
    void printLineHighlight(Span span, TermColor color, std::ostream& os = std::cerr) const;
    void printHighlight(Span span, TermColor color, std::ostream& os = std::cerr) const;
};

struct Code::Cursor {
    std::shared_ptr<const Code> code;
    int lno;
    int cno;

    Cursor() = default;
    Cursor(std::shared_ptr<const Code> code, int lno, int cno);
    Cursor &operator++();
    Cursor operator++(int);
    Cursor &operator--();
    Cursor operator--(int);
    char operator*() const;
    bool operator!=(const Cursor &other) const;
    bool operator==(const Cursor &other) const;
    Cursor &skipLine();
    Cursor &skipPrevLine();
    bool bof() const;
    bool eof() const;
    friend std::ostream &operator<<(std::ostream &os, const Cursor &cs);
};

struct Code::Span {
    std::shared_ptr<const Code> code;
    int st_lno;
    int st_cno;
    int ed_lno;
    int ed_cno;

    Span() = default;
    Span(std::shared_ptr<const Code> code, int st_lno, int st_cno, int ed_lno,
         int ed_cno);
    Span(std::shared_ptr<const Code> code, const Cursor &st, const Cursor &ed);
    bool operator==(const Span &other) const;
    bool operator!=(const Span &other) const;
    Cursor begin() const;
    Cursor end() const;
    std::string str() const;
    void extend();
    void extendLine();
    void collapse();
    bool empty() const;
    bool eof() const;
    friend std::ostream &operator<<(std::ostream &os, const Span &span);
};

#endif
