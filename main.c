#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

// STEPS: 
// 1. Enter Raw Mode
// 2. Handle Raw I/O
// 3. Create Text Viewer Window/Screen
// 4. Create the editor (main features)

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  // Turn off:
  raw.c_lflag &= ~(ECHO);
  raw.c_lflag &= ~(ICANON); // canonical mode 
  raw.c_lflag &= ~(ISIG); // ctrl-c, ctrl-z
  raw.c_lflag &= ~(ICRNL); // carriage return -> see report
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char *argv[]){
	
	
	return 0;
}
