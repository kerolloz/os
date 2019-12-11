
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

void welcome() {
  printf(" _   _ _____ _     _     ___   \n"
         "| | | | ____| |   | |   / _ \\ \n"
         "| |_| |  _| | |   | |  | | | | \n"
         "|  _  | |___| |___| |__| |_| | \n"
         "|_| |_|_____|_____|_____\\___/ \n");
}

bool isChildProcess(pid_t pid) { return pid == 0; }

struct command {
  string s;
  vector<char *> argschar;
  bool hasPipe;
  command(string s) : s(s) {
    vector<string> args;
    split(args);
    for (int i = 0; i < args.size(); i++) {
      char *arg = const_cast<char *>(args[i].c_str());
      cout << arg << endl;
      argschar.push_back(arg);
    }
  }
  void split(vector<string> &commands) {
    hasPipe = true;
    string str = "";
    for (int i = 0; i < s.size(); i++) {
      if (isspace(s[i])) {
        if (str.size())
          commands.push_back(str);
        str = "";
      } else
        str += s[i];
    }
    if (str.size())
      commands.push_back(str);
  }
  const char *get_command() { return argschar[0]; }
  char *const *get_args() { return argschar.data(); }
  void execute() { execvp(get_command(), get_args()); }
};

int main() {
  welcome();
  string user_input;
  while (1) {
    printf("\n> ");

    getline(cin, user_input);
    if (cin.eof() || user_input == "exit") {
      // if pressed CTRL + D "EOF" or entered exit
      break;
    }
    pid_t process_id = fork();
    if (isChildProcess(process_id)) {
      command cmd(user_input);
      cmd.execute();
      printf("ERROR\n");
      return 0; // return zero to stop the child process from continue if failed
    }
    wait(NULL);
  }
  printf("\nOkay, Bye!\n");
  return 0;
}
