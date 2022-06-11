#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "cursorPosition.h"

int findWinSz(int *rows, int *cols) {
  struct winsize window_size;
  int status = ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size);
  if(status == -1 || window_size.ws_col == 0){
  		return findCursor(rows, cols);
  } 
  else{
        *cols = window_size.ws_col;
        *rows = window_size.ws_row;
        return 0;
  }
}
