#ifndef GH_INPUT_H
#define GH_INPUT_H

void waitForESC();
int gh_getch();
int gh_kbhit();
int gh_getkey();

#define GH_UP     1
#define GH_DOWN   2
#define GH_LEFT   3
#define GH_RIGHT  4
#define GH_ENTER  5
#define GH_ESC    6

#endif
