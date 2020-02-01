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
    // prompt **in color**
    printf("%s{shell:~}%s > %s", KCYN, KYEL, KNRM); 
    
    input = read_line();

    if (!strcmp("exit", input)) {
      running = 0;
      continue;
    }

    int scannedLength = strlen(input);
    int printOutput = 1;

    // set the flag to print or not
    if (input[scannedLength - 1] == '&') {
      input[scannedLength - 1] = '\0'; // remove the ampersand before passing to exec;
      printOutput = 0;
    }

    args = split_line(input);
    pid = fork();
    if (pid < 0) {
      printf("errored in fork\n");
      exit(-1);
    } 
    else if (pid == 0) {
      if (!printOutput) {
        int fd = open("/dev/null", O_WRONLY);

        dup2(fd, 1);    /* make stdout a copy of fd (> /dev/null) */
        dup2(fd, 2);    /* ...and same with stderr */
        close(fd);      /* close fd */
      }
      execvp(args[0], args);
      exit(0);
    } 
    else {
      if (printOutput) {
        do {
          wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
      } else {
        printf("Running command in background with pid: %d", pid);
      }
    }
    counter++;
  }

  printf("%sTerminating shell, %i commands inputted\n%s", KYEL, counter, KNRM);
  exit(0);
}
