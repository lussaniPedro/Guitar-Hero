#include "gh_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
#include <conio.h>

int gh_getch(){ return getch(); }
int gh_kbhit(){ return _kbhit(); }

int gh_getkey(){
    int c = gh_getch();
    if(c == 224){ // Teclas especiais
        c = gh_getch();
        switch(c){
            case 72: return GH_UP;
            case 80: return GH_DOWN;
            case 75: return GH_LEFT;
            case 77: return GH_RIGHT;
        }
    } else if(c == 13) return GH_ENTER;
    else if(c == 27) return GH_ESC;
    else return c;
}

#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int gh_getch(){
    struct termios oldt, newt;

    char ch;
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int gh_kbhit(){
    struct termios oldt, newt;
    int oldf, ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int gh_getkey(){
    int c = gh_getch();
    if(c == 27){
        if(gh_kbhit()){
            int c2 = gh_getch();
            if(c2 == '['){
                int c3 = gh_getch();
                switch(c3){
                    case 'A': return GH_UP;
                    case 'B': return GH_DOWN;
                    case 'C': return GH_RIGHT;
                    case 'D': return GH_LEFT;
                }
            }
        }

        return GH_ESC;
    } else if(c == 10) return GH_ENTER;
    else return c;
}

#endif

void waitForESC(){
    char c;

    do{
        c = gh_getch();
    } while(c != 27);
}