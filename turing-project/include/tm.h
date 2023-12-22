#ifndef __TURING_H__
#define __TURING_H__

#include "code.h"
#include <memory>
#include <string_view>
#include <vector>

class TuringMachine {
    friend class Parser;

  private:
    std::vector<std::string> Q;

  public:
    void parse(std::shared_ptr<Code> code);
    void run(std::string_view input, bool verbose = false);
};

#endif
