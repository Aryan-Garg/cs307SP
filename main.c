#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"
#include "modeChanger.h"

// STEPS: 
// 1. Enter Raw Mode -- DONE!
// 2. Handle Raw I/O
// 3. Create Text Viewer Window/Screen
// 4. Create the editor (main features)

int main(int argc, char *argv[]){
	enterRawMode();
	while (1) {
		char ch = '\0';
		if (read(STDIN_FILENO, &ch, 1) == -1 && errno != EAGAIN) 		throwErrorMsg("read not working in main loop");
		
		if (iscntrl(ch)) printf("%d\r\n", ch);
		else printf("%d ('%c')\r\n", ch, ch);
    
		if (ch == 'q') break;
	}

	
	return 0;
}
