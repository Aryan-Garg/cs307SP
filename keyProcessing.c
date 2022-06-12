#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"
#define ctrl_key(k) ((k) & 0x1f)

char keyRead() {
  int bytes_read;
  char c;
  while ((bytes_read=read(STDIN_FILENO, &c, 1))!=1) {
    if (bytes_read==-1 && errno!=EAGAIN){
		throwErrorMsg("read");
	}
  }
  return c;
}


void moveCursorEditor(char key) {
  if(key=='a' && editor.curs_x > 0) {
      editor.curs_x--;
  }
  else if(key=='d' && editor.curs_x < editor.number_of_cols - 1) {
      editor.curs_x++;
  }
  else if(key=='w' && editor.curs_y > 0) {
      editor.curs_y--;
  }
  else if(key=='s' && editor.curs_y < editor.number_of_rows - 1) {
      editor.curs_y++;
  }
}
if (E.cx != E.screencols - 1)

void keypressEditor() {
  char c = keyRead();
  if(c==ctrl_key('q')){
  	  exit(0);
  }
  else if(c=='w' || c=='a' || c=='s' || c=='d'){
	  moveCursorEditor(c);
  }
  
}
