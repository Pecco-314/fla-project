#ifndef __TURING_H__
#define __TURING_H__

#include <string_view>
#include <vector>

class TuringMachine {
    friend class Parser;

  private:
    std::vector<std::string> Q;

  public:
    void parse(std::string_view path, bool verbose = false);
    void run(std::string_view input, bool verbose = false);
};

#endif
