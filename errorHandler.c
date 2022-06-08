#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void throwErrorMsg(char *strstr) {
  perror(strstr);
  exit(1);
}