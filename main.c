#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

#include "errorHandler.h"
#include "modeChanger.h"

// STEPS: 
// 1. Enter Raw Mode
// 2. Handle Raw I/O
// 3. Create Text Viewer Window/Screen
// 4. Create the editor (main features)

int main(int argc, char *argv[]){
	enableRawMode();
	while (1) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) 		throwErrorMsg("read not working in main loop");
		
		if (iscntrl(c)) printf("%d\r\n", c);
		else printf("%d ('%c')\r\n", c, c);
    
		if (c == 'q') break;
	}

	
	return 0;
}
