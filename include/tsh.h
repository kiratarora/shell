#ifndef _SIMPLE_SHELL_H
#define _SIMPLE_SHELL_H
#define MAX_COMMAND_ARGS 128
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <vector>

class simple_shell {
 public:
  std::vector<std::vector<char*>> parse_command(char* cmd);
  void exec_command(std::vector<std::vector<char*>> commands);
  bool isQuit(char* cmd);
  void cd_command(char** argv);
};

#endif