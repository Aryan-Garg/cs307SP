#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"

struct termios orig;

void quitRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig) == -1) throwErrorMsg("tcsetattr");
}

void enterRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig) == -1) throwErrorMsg("tcgetattr");
	atexit(quitRawMode);
	
	struct termios orig_copy = orig;
	
	// Turn off:
	
	//Try cfmakeraw() to set all flags at once
	orig_copy.c_lflag &= ~(ECHO);
	orig_copy.c_lflag &= ~(ICANON); // canonical mode 
	orig_copy.c_lflag &= ~(ISIG); // ctrl-c, ctrl-z
	orig_copy.c_lflag &= ~(ICRNL); // carriage return -> report
	orig_copy.c_oflag &= ~(OPOST); // output processing -> report
	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_copy) == -1) 
		throwErrorMsg("tcsetattr");
}
