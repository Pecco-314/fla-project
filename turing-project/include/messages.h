#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>

namespace msg {

const inline std::string USAGE = "Usage: turing [-v|--verbose] [-h|--help] [-c|--color] <tm_path> <input>";
const inline std::string INVALID_OPTION = "Invalid option: {}";
const inline std::string TOO_MANY_ARGS = "Too many arguments";
const inline std::string TOO_FEW_ARGS = "Too few arguments";
const inline std::string FILE_NOT_EXIST = "{} does not exist";
const inline std::string NOT_A_REGULAR_FILE = "{} is not a regular file";
const inline std::string UNABLE_TO_READ = "Unable to read {}";

} // namespace msg

#endif
