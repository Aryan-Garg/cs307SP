#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"

#define ctrl_key(k) ((k) & 0x1f)

struct termios orig;

void throwErrorMsg(char *strstr) {
  perror(strstr);
  exit(1);
}

void quitRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) throwErrorMsg("tcsetattr");
}

void enterRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig) == -1) throwErrorMsg("tcgetattr");
	atexit(quitRawMode);
	
	struct termios orig_copy = orig;
	
	// Turn off:
	orig_copy.c_lflag &= ~(ECHO);
	orig_copy.c_lflag &= ~(ICANON); // canonical mode 
	orig_copy.c_lflag &= ~(ISIG); // ctrl-c, ctrl-z
	orig_copy.c_lflag &= ~(ICRNL); // carriage return -> report
	orig_copy.c_oflag &= ~(OPOST); // output processing -> report
	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_copy) == -1) 
		throwErrorMsg("tcsetattr");
}

int main() {
  enterRawMode();

  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) 
        throwErrorMsg("read");

    if (iscntrl(c)) {
      printf("%d\r\n", c);

    } else {
      printf("%d ('%c')\r\n", c, c);
    }

    if (c == ctrl_key('q')) 
        break;
  }

  return 0;
}