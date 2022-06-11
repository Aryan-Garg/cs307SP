#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include "errorHandler.h"
#include "modeChanger.h"
#include "keyProcessing.h"
#include "editorSpecs.h"
#include "cursorPosition.h"

#define ctrl_key(k) ((k) & 0x1f)


// STEPS: 
// 1. Enter Raw Mode -- DONE!
// 2. Handle Raw I/O
// 3. Create Text Viewer Window/Screen
// 4. Create the editor (main features)

struct EditorData{
	int curs_x, curs_y;
	int number_of_rows, number_of_cols;
	struct termios orig;
};
struct EditorData editor;

struct appendBuffer{
	char *ptrBuf;
	int len;
};	

void appendBuffer_concat(struct appendBuffer *ab2, const char *s, int len) {
	char *newBuf = realloc(ab2->ptrBuf, ab2->len + len); // increase buf size
	if (newBuf == NULL) return;
	memcpy(&newBuf[ab2->len], s, len);
	ab2->ptrBuf = newBuf;
	ab2->len += len;
}

// Destructor: appendBuffer
void appendBuffer_free(struct appendBuffer *ab2) {
	free(ab2->ptrBuf);
}

void init(){
	editor.curs_x = 0;
	editor.curs_y = 0;
	int status = findWinSz(&editor.number_of_rows,&editor.number_of_cols);
	if(status==-1){
		throwErrorMsg("findWinSz");
	}
}

void drawEditor(struct appendBuffer *appBuff) {
  for (int i = 0; i < editor.number_of_rows; i++) {
    appendBuffer_concat(appBuff, "~", 1);
	appendBuffer_concat(appBuff, "\x1b[K", 3); // clear each line before redrawing
    if (i < editor.number_of_rows - 1) appendBuffer_concat(appBuff, "\r\n", 2);
  }
}

void refreshEditor() {
  struct appendBuffer AB = {NULL, 0}; // constructor
  
  // Anukool: Check VT100 manual...
  appendBuffer_concat(&AB, "\x1b[?25l", 6); // hide cursor: VT100
 
  appendBuffer_concat(&AB, "\x1b[H", 3);
  drawEditor(&AB);
  appendBuffer_concat(&AB, "\x1b[H", 3);
  appendBuffer_concat(&AB, "\x1b[?25h", 6); // show/reset cursor: VT100
  
  write(STDOUT_FILENO, AB.ptrBuf, AB.len);
  appendBuffer_free(&AB);
}

void setTerminal() {
  struct appendBuffer AB = {NULL, 0}; // constructor
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  drawEditor(&AB);
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
