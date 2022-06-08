#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"

struct termios orig_termios;

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) throwErrorMsg("tcsetattr");
}

void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) 
		throwErrorMsg("tcgetattr");
	atexit(disableRawMode);
	struct termios raw = orig_termios;
	// Turn off:
	raw.c_lflag &= ~(ECHO);
	raw.c_lflag &= ~(ICANON); // canonical mode 
	raw.c_lflag &= ~(ISIG); // ctrl-c, ctrl-z
	raw.c_lflag &= ~(ICRNL); // carriage return -> report
	raw.c_oflag &= ~(OPOST); // output processing -> report
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) 
		throwErrorMsg("tcsetattr");
}