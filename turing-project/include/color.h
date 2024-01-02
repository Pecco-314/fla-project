#ifndef __COLOR_H__
#define __COLOR_H__

#include <ostream>
#include <vector>

class TermColor {
  private:
    std::vector<int> colors;
    static bool force_color;

  public:
    TermColor() = default;
    TermColor(int color...) : colors{color} {}
    TermColor operator|(const TermColor &other) const;
    std::string str() const;
    friend std::ostream &operator<<(std::ostream &os, const TermColor &color);
    static void setForceColor(bool force_color = true);
};

const inline TermColor BLACK = {30};
const inline TermColor RED = {31};
const inline TermColor GREEN = {32};
const inline TermColor YELLOW = {33};
const inline TermColor BLUE = {34};
const inline TermColor MAGENTA = {35};
const inline TermColor CYAN = {36};
const inline TermColor WHITE = {37};
const inline TermColor RESET = {0};
const inline TermColor BOLD = {1};
const inline TermColor UL = {4};

class Painted {
  private:
    std::string_view text;
    TermColor color;

  public:
    Painted() = default;
    Painted(std::string_view text, TermColor color) : text(text), color(color) {}
    std::string str() const;
    friend std::string operator+(std::string_view str, const Painted &paint);
    friend std::string operator+(const Painted &paint, std::string_view str);
    friend std::string operator+(const Painted &paint1, const Painted &paint2);
    friend std::ostream &operator<<(std::ostream &os, const Painted &paint);
};

const inline Painted ERR = {"error: ", RED | BOLD};
const inline Painted WARN = {"warning: ", YELLOW | BOLD};
const inline Painted NOTE = {"note: ", BLUE | BOLD};
const inline Painted FAILED = {"failed: ", RED | BOLD};
const inline Painted PASSED = {"passed: ", GREEN | BOLD};
const inline Painted INPUT = {"Input: ", BLUE | BOLD};
const inline Painted RESULT = {"Result: ", BLUE | BOLD};
const inline Painted ACC = {"ACCEPTED", GREEN | BOLD};
const inline Painted ACC_Q = {"(ACCEPTED)", GREEN | BOLD};
const inline Painted UNACC = {"UNACCEPTED", RED | BOLD};
const inline Painted UNACC_Q = {"(UNACCEPTED)", RED | BOLD};

#endif
