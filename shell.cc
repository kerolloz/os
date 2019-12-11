
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

void hello() {
  printf(" _   _ _____ _     _     ___   \n"
         "| | | | ____| |   | |   / _ \\ \n"
         "| |_| |  _| | |   | |  | | | | \n"
         "|  _  | |___| |___| |__| |_| | \n"
         "|_| |_|_____|_____|_____\\___/ \n");
}

void bye() {
  printf("\n"
         " ______   _______ \n"
         "| __ ) \\ / / ____|\n"
         "|  _ \\\\ V /|  _|  \n"
         "| |_) || | | |___ \n"
         "|____/ |_| |_____|\n");
}

bool isChildProcess(pid_t pid) { return pid == 0; }

struct command {
  string command_string;
  vector<char *> argschar;
  command(string s) : command_string(s) {
    vector<string> args;
    split(args);
    for (const string &arg : args) {
      char *str = const_cast<char *>(arg.c_str());
      argschar.push_back(str);
    }
  }
  void split(vector<string> &commands) {
    // just like python split
    // intput(string): "ls -la --color"
    // output(array of strings): ["ls", "-la", "--color"]
    string tmp_str = "";
    for (char c : command_string) {
      if (isspace(c)) {
        if (tmp_str.size())
          commands.push_back(tmp_str);
        tmp_str = "";
      } else
        tmp_str += c;
    }
    if (tmp_str.size())
      commands.push_back(tmp_str);
  }
  const char *get_command() { return argschar[0]; }
  char *const *get_args() { return argschar.data(); }
  void execute() {
    /*
    we use execvp because it takes an array as the second argument
    this will ease sending arguments
    execvp searches in the `PATH' environment variable if the first argument
    contains no slashes
    */
    execvp(get_command(), get_args());
  }
};

int main() {
  hello();
  string user_input;
  while (1) {
    printf("\n[shell@user]$ ");

    getline(cin, user_input);
    if (cin.eof() || user_input == "exit") {
      // if pressed CTRL + D "EOF" or entered "exit"
      break;
    }
    pid_t process_id = fork();
    if (process_id < 0) {
      printf("Failed to fork");
      exit(-1);
    }
    if (isChildProcess(process_id)) {
      command cmd(user_input);
      // (execute) replaces the child process image with the command to execute
      cmd.execute();
      // the following lines will execute, IFF command failed to execute
      printf("ERROR executing: %s\n", user_input.c_str());
      exit(0); // stop the child process, if failed
    }
    wait(NULL); // wait for the child to finish execution
  }
  bye();
  return 0;
}
