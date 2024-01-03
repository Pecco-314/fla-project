#include "tape.h"
#include <iomanip>
#include <iostream>

Tape::Tape(int id) : id(id), head(0) {}

Tape::Tape(int id, std::string_view input) : id(id), head(0) {
    for (auto c : input) {
        right.push_back(c);
    }
}

char Tape::get(int index) const {
    if (index < 0) {
        if (-index - 1 >= (int)left.size()) {
            return '_';
        } else {
            return left[-index - 1];
        }
    } else {
        if (index >= (int)right.size()) {
            return '_';
        } else {
            return right[index];
        }
    }
}

char Tape::get() const {
    return get(head);
}

void Tape::set(int index, char ch) {
    if (index < 0) {
        if (-index - 1 >= (int)left.size()) {
            left.resize(-index, '_');
        }
        if (ch != '*') { left[-index - 1] = ch; }
    } else {
        if (index >= (int)right.size()) {
            right.resize(index + 1, '_');
        }
        if (ch != '*') { right[index] = ch; }
    }
}

void Tape::set(char ch) {
    set(head, ch);
}

void Tape::move(char dir) {
    if (dir == 'l') {
        head--;
    } else if (dir == 'r') {
        head++;
    } else if (dir == '*') {
        return;
    }
}

bool Tape::match(char ch) const {
    return get() == ch || (ch == '*' && get() != '_');
}

int Tape::leftmost() const {
    return -(int)left.size();
}

int Tape::rightmost() const {
    return (int)right.size() - 1;
}

void Tape::log() const {
    int lm = leftmost() - 1, rm = rightmost() + 1;
    while (lm < head && get(lm) == '_') { lm++; }
    while (rm > head && get(rm) == '_') { rm--; }
    if (lm > rm) { lm = rm = head; }

    std::cout << "Index" << std::setw(2) << std::left << id << ": ";
    for (int i = lm; i <= rm; i++) {
        std::cout << abs(i) << " ";
    }
    std::cout << std::endl;

    std::cout << "Tape" << std::setw(3) << std::left << id << ": ";
    for (int i = lm; i <= rm; i++) {
        int w = std::to_string(abs(i)).length() + 1;
        std::cout << std::setw(w) << std::left << get(i);
    }
    std::cout << std::endl;

    std::cout << "Head" << std::setw(3) << std::left << id << ": ";
    for (int i = lm; i <= rm; i++) {
        int w = std::to_string(abs(i)).length() + 1;
        std::cout << std::setw(w) << std::left;
        if (i == head) {
            std::cout << "^";
        } else {
            std::cout << "";
        }
    }
    std::cout << std::endl;
}

std::string Tape::str() const {
    std::string s;
    int lm = leftmost(), rm = rightmost();
    while (lm <= rm && get(lm) == '_') { lm++; }
    while (rm >= lm && get(rm) == '_') { rm--; }
    for (int i = lm; i <= rm; i++) {
        s += get(i);
    }
    return s;
}
