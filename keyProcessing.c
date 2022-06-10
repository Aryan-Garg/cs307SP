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

void keypressEditor() {
  char c = keyRead();
  if(c==ctrl_key('q')){
  	  exit(0);
  }
  
}
