#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include "errorHandler.h"
#include "modeChanger.h"
#include "keyProcessing.h"
#include "editorSpecs.h"

#define ctrl_key(k) ((k) & 0x1f)

// STEPS: 
// 1. Enter Raw Mode -- DONE!
// 2. Handle Raw I/O
// 3. Create Text Viewer Window/Screen
// 4. Create the editor (main features)

struct EditorData{
	int number_of_rows, number_of_cols;
	struct termios orig;
};
struct EditorData editor;

void init(){
	int status = findWinSz(&editor.number_of_rows,&editor.number_of_cols);
	if(status==-1){
		throwErrorMsg("findWinSz");
	}
}

void drawEditor() {
  int counter;
  for (counter=0; counter<24; counter++) {
    write(STDOUT_FILENO, ">\r\n", 3);
  }
}


void setTerminal() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  drawEditor();
  write(STDOUT_FILENO, "\x1b[H", 3);
}

int main(int argc, char *argv[]){
	enterRawMode();
	init();
	while (1) {
		setTerminal();
		keypressEditor();
	}
	return 0;
}
