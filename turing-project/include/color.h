#ifndef __COLOR_H__
#define __COLOR_H__

#include <ostream>
#include <vector>

class TermColor {
  private:
    std::vector<int> colors;

  public:
    TermColor() = default;
    TermColor(int color...) : colors{color} {}
    TermColor operator|(const TermColor &other) const;
    friend std::ostream &operator<<(std::ostream &os, const TermColor &color);
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
    friend std::ostream &operator<<(std::ostream &os, const Painted &paint);
};

const inline Painted ERR = {"error: ", RED | BOLD};
const inline Painted WARN = {"warning: ", YELLOW | BOLD};
const inline Painted NOTE = {"note: ", BLUE | BOLD};
const inline Painted FAILED = {"failed: ", RED | BOLD};
const inline Painted PASSED = {"passed: ", GREEN | BOLD};

#endif
