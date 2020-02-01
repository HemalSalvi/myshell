//myshell.c by Hemal Salvi and Al Madireddy

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

char* read_line(void) {
  int bufferSize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufferSize); // allocate 1024 chars
  int c;

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';  // null terminate our string
      return buffer;
    } else {
      buffer[position] = c; // add char to the buffer
    }
    position ++;

    // if the command is really long, 
    // allocate more space for the command string
    if (position >= bufferSize) {
      bufferSize += 1024;
      buffer = realloc(buffer, bufferSize);
    }
  }
}

char** split_line(char* line) {
  int bufferSize = 64, position = 0;
  char** tokens = malloc(bufferSize * sizeof(char*));
  char* token;

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufferSize) {
      bufferSize += 64;
      tokens = realloc(tokens, bufferSize * sizeof(char*));
    }
    token = strtok(NULL, " \t\r\n\a");
  }

  tokens[position] = NULL;
  return tokens;
}


int main(void) {
  int status, counter = 0; // count instructions
  int running = 1; // boolean int where 1 means keep running; tracking when to exit loop
  int MAX_INPUT_LENGTH = 100;
  char* input;
  char** args;
  pid_t pid, wpid;

  printf("Welcome"); 
  while (running == 1) {
    printf("\n"); 
    printf("%s{shell:~}%s > %s", KCYN, KYEL, KNRM); 
    
    // read line and check that it's not "exit"
    input = read_line();
    if (!strcmp("exit", input)) {
      running = 0;
      continue;
    }

    int scannedLength = strlen(input);
    int printOutput = 1;

    if (input[scannedLength - 1] == '&') {
      input[scannedLength - 1] = '\0';
      printOutput = 0;
    }

    // tokenize the input string and split on whitespace
    // so that args can be passed to execvp
    args = split_line(input);
 
    pid = fork();
    if (pid < 0) {
      printf("errored in fork\n");
      exit(-1);
    } 
    else if (pid == 0) {
      // child process code: execute the command
      // Write to /dev/null to prevent output of 
      // command if it's running in background with '&'
      if (!printOutput) {
        int fd = open("/dev/null", O_WRONLY);

        dup2(fd, 1);  // stdout
        dup2(fd, 2);  // stderr
        close(fd);
      }

      // pass in the command and the arguments to run
      execvp(args[0], args);
      exit(0);
    } 
    else {
      // parent process code
      if (printOutput) {
        do {
          wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
      } 
      else {
        printf("Running command in background with pid: %d", pid);
      }
    }
    counter++;
  }

  printf("%sTerminating shell,%s %i %scommands inputted\n%s", KYEL, KCYN, counter, KYEL, KNRM);
  exit(0);
}
