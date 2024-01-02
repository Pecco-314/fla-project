#ifndef __TAPE_H__
#define __TAPE_H__

#include <string_view>
#include <vector>

class Tape {
  private:
    int id;
    std::vector<char> left;
    std::vector<char> right;
    int head;

  public:
    Tape() = default;
    Tape(int id);
    Tape(int id, std::string_view input);
    char get(int index) const;
    char get() const;
    bool match(char ch) const;
    int leftmost() const;
    int rightmost() const;
    void set(char ch);
    void set(int index, char ch);
    void move(char dir);
    void log() const;
    std::string str() const;
};

#endif
