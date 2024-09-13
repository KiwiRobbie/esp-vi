#include "mcurses-config.h"

int tigetnum(char* property) ;
char* tigetstr(char* property) ;


int tputs(const char *str, int affcnt, int (*putc)(int));
char *tgoto(const char *cap, int col, int row);

void
addnstr (const char * str, int n);

#define _POSIX_VDISABLE -1;
#define ECHOCTL     0x00200
#define ECHOKE	0x00800