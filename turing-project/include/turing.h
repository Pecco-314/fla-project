#ifndef __TURING_H__
#define __TURING_H__

#include <string>
class TuringMachine {
  private:
    std::string tm_path;
    bool verbose;

  public:
    TuringMachine(const std::string &tm_path, bool verbose);
    void run(const std::string &input);
};

#endif
