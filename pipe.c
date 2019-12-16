#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  // We use pipes to send output from child to parent

  int fd[2]; // Used to store two ends of pipe
  pipe(fd);
  if (fork() == 0) {
    // child process
    char concat_str[100];
    printf("Hello from child, enter a string: ");
    scanf("%s", concat_str);
    write(fd[1], concat_str, strlen(concat_str) + 1);

    exit(0);
  } else {
    // Parent process
    wait(NULL); // wait the child to finish
    char concat_str[100];
    read(fd[0], concat_str, 100);
    printf("Hello from parent, this is the string you entered: %s\n",
           concat_str);
  }
}