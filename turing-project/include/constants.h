#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"
#define UL      "\033[4m"
#define RESET   "\033[0m"
#define ERR     RED BOLD "error: " RESET
#define WARN    YELLOW BOLD "warning: " RESET
#define NOTE    BLUE BOLD "note: " RESET

#endif
