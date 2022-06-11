app: main.c keyProcessing.c errorHandler.c modeChanger.c editorSpecs.c cursorPosition.c
	gcc -o app main.c keyProcessing.c errorHandler.c modeChanger.c editorSpecs.c cursorPosition.c -Wall
