#include <string.h>
int gethostname(char *name, size_t len){ 
    strncpy(name, "kiwi-esp", len-1);
    name[len -1] ='\0';
    return 0;
}
