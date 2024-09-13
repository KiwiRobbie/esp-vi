#include <string.h>
#include "mcurses.h"
#include "mcurses-config.h"
int tigetnum(const char* property) 
{
    if (strcmp(property, "lines")) {
        return MCURSES_LINES;
    }
    else if (strcmp(property, "cols")) {
        return MCURSES_COLS;
    } else {
        return -1;
    }
}

int tigetstr(const char* property) 
{
    return -1;
}

int tputs(const char *str, int affcnt, int (*putc)(int)){
    for (char* t = str; *t != '\0'; t++) {
        putc((int)t);
    }
    return 0;
}
char *tgoto(const char *cap, int col, int row){
    return "patched";
}

void
addnstr (const char * str, int n)
{
    if( n == 0) { 
        addstr(str);
        return; 
    }

    for(int i =0; i<n;i++){
        if(*str) {
            addch(*str++);
        } else {
            break;
        }
    }

}