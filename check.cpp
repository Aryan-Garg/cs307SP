#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <fstream>
#include <conio.h>

void ClearScreen() {
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void ScrollUp()
{

}

void ScrollDown()
{
    
}
void ScrollLeft()
{
    
}
void ScrollRight()
{
    
}


int main()
{
    ClearScreen();
    int screencols = 150; //window size in number of character
    int screenrows = 40;
    int currentcol = 0; //cursor coords
    int currentrow = 0;
    int firstcol = 0; //in-file coordinates of the first character shown on screen
    int firstrow = 0;

    //char screen_text_buffer[40][150] {'\0'}; //initialize char array as NUL character "\0"

    char cur_char;

    std::ifstream file("Sample_text/sample.txt");
    std::string fileline;
    std::string screen_text_buffer[40];
    int nooflines = 0;
    if (file.is_open() ) {
        while (file && nooflines < screenrows) {        // take 'screenrows' rows into buffer
            std::getline (file, screen_text_buffer[nooflines] );
            //strcpy(screen_text_buffer[row_index], fileline.c_str());
            nooflines++;
        }
    }
    else {
        std::cout << "Couldn't open file! Please check the file name.\n";
    }


    // Viewer mode
    char curcommand;
    for(int i = 0; i < screenrows; i++)
    {
        int linelen = screen_text_buffer[i].length();
        //for(int j = 0; j < screencols && j < linelen; j++)
        //{
            //if(disp=='\0') break;
        std::cout << screen_text_buffer[i];
        //}
        if (i!=screenrows) std::cout << '\n';
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // (Works with Windows)
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
    SetConsoleScreenBufferSize(hConsole,(COORD) {1000,40});

    while(curcommand != 'q')
    {
        // Viewer mode, go to type mode with 't'

        // display text

        //std::cin >> cur_char;
        //WriteConsole(hConsole, "\nHello\n", 5, NULL, NULL);
        DWORD mode = 0;
        GetConsoleMode(hConsole, &mode);
        SetConsoleMode(hConsole, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_WRAP_AT_EOL_OUTPUT));
        curcommand = getch();
        //std::cout << curcommand;
        if(curcommand == 'a')
        {
            currentcol--;
            if(currentcol<0)
            {
                currentcol++;
                firstcol--;
                if(firstcol<0)
                {
                    firstcol++;
                }
                else
                {
                    ScrollLeft();
                    // ClearScreen();
                    // for(int i = 0; i < screenrows; i++)
                    // {
                    //     int linelen = screen_text_buffer[i].length();
                    //     for(int j = 0; j < screencols && j < linelen-firstcol; j++)
                    //     {
                    //         //if(disp=='\0') break;
                    //         std::cout << screen_text_buffer[i][firstcol+j];
                    //     }
                    //     if (i!=screenrows) std::cout << '\n';
                    // }
                }

            }
            pos = {(SHORT) currentcol,(SHORT) currentrow};
            SetConsoleCursorPosition(hConsole, pos);
        }
        if(curcommand == 'd')
        {
            currentcol++;
            if(currentcol>=screencols || currentcol >= screen_text_buffer[currentrow].length())
            {
                currentcol--;
                firstcol++;
                if(firstcol>=screen_text_buffer[currentrow].length())
                {
                    firstcol--;
                }
                else
                {
                    ScrollRight();
                    // ClearScreen();
                    // for(int i = 0; i < screenrows; i++)
                    // {
                    //     int linelen = screen_text_buffer[i].length();
                    //     for(int j = 0; j < screencols && j < linelen-firstcol; j++)
                    //     {
                    //         //if(disp=='\0') break;
                    //         std::cout << screen_text_buffer[i][firstcol+j];
                    //     }
                    //     if (i!=screenrows) std::cout << '\n';
                    // }
                }

            }
            pos = {(SHORT) currentcol,(SHORT) currentrow};
            SetConsoleCursorPosition(hConsole, pos);
        }
        if(curcommand == 'w')
        {
            currentrow--;
            if(currentrow<0)
            {
                currentrow++;
                ScrollUp();
            }
            pos = {(SHORT) currentcol,(SHORT) currentrow};
            SetConsoleCursorPosition(hConsole, pos);
        }
        if(curcommand == 's')
        {
            currentrow++;
            if(currentrow>=screenrows)
            {
                currentrow--;
                ScrollDown();
            }
            pos = {(SHORT) currentcol,(SHORT) currentrow};
            SetConsoleCursorPosition(hConsole, pos);
        }
        if(curcommand == 't')
        {
            // Type mode, exit with Esc (27)
            while((curcommand = getch()) != 27) 
            {
                
            }
        }
    }
    ClearScreen();
    return 0;
}