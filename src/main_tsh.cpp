#include <tsh.h>

using namespace std;

int main() {
  char cmd[81];
  // std::vector<char **> commands;
  std::vector<std::vector<char*>> commands;
  simple_shell *shell = new simple_shell();
  cout << "tsh> ";
  while (fgets(cmd, sizeof(cmd), stdin)) {
    if (cmd[0] != '\n') { 
      commands = shell->parse_command(cmd); 
      shell->exec_command(commands);
    }
    cout << "tsh> ";
  }
  cout << endl;
  exit(0);
}