#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void throwErrorMsg(char *strstr) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(strstr);
  exit(1);
}
