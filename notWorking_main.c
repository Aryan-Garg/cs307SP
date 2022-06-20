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
// 2. Handle Raw I/O -- DONE!
// 3. Create Text Viewer Window/Screen -- No scrolling and escape seqs, else DONE!
// 4. Create the editor (main features)

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

typedef struct editor_row {
  int len;
  char *chars;
} editor_row;


struct EditorData{
	int curs_x, curs_y;
	int number_of_rows, number_of_cols;
	struct termios orig;
	editor_row *erow;
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
    editor.erow = NULL;
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

void appendToEditor(char *s, size_t len) {
  editor.erow = realloc(E.row, sizeof(erow) * (editor.typedrows + 1));
  int app_index = editor.typedrows;
  editor.erow[app_index].size = len;
  editor.erow[app_index].chars = malloc(len + 1);
  memcpy(editor.erow[app_index].chars, s, len);
  editor.erow[app_index].chars[len] = '\0';
  editor.typedrows++;
}

void openfile(char *fname) {
  FILE *file = fopen(fname, "r");
  if (!file) throwErrorMsg("fopen");
  char *line = NULL;
  size_t line_capacity = 0;
  ssize_t length;
  while ((length = getline(&line, &line_capacity, fp)) != -1) {
    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) length--;
    appendToEditor(line, length);
  }
  free(line);
  fclose(file);
}

int main(int argc, char *argv[]){
	enterRawMode();
	init();
	if (argc>1) openfile(argv[1]);
	
	while (1) {
		setTerminal();
		keypressEditor();
	}
	return 0;
}
