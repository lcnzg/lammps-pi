#include "arg_parse.h"

signed int parse_init_(char* arg1, char* arg2) {

  int max = atoi(arg2);

  if (strncmp(arg1, "-max-pi", 7) == 0) {
    return max;
  }

  return -1;
}