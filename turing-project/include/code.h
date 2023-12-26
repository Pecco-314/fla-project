#ifndef __CODE_H__
#define __CODE_H__

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Code : public std::enable_shared_from_this<Code> {
  private:
    std::string path_;
    std::vector<std::string> lines_;

  public:
    struct Cursor;
    struct Span;
    Code(std::string_view path);
    std::string_view path() const;
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
    void printLineHighlight(Span span, const char *color) const;
    void printHighlight(Span span, const char *color) const;
};

struct Code::Cursor {
    std::shared_ptr<const Code> code;
    int lno;
    int cno;

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
};

struct Code::Span {
    std::shared_ptr<const Code> code;
    int st_lno;
    int st_cno;
    int ed_lno;
    int ed_cno;

    Span(std::shared_ptr<const Code> code, int st_lno, int st_cno, int ed_lno,
         int ed_cno);
    Span(std::shared_ptr<const Code> code, const Cursor &st, const Cursor &ed);
    bool operator==(const Span &other) const;
    Cursor begin() const;
    Cursor end() const;
    std::string str() const;
    void extend();
    void extendLine();
    void collapse();
    bool empty() const;
    bool eof() const;
};

#endif
