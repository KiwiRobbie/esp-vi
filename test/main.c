#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    // struct stat sb; 
    FILE* file = fopen("./test.tmp", "r+");
    int fd = fileno(file);
    char* buf[16];
    fprintf(file, "hello word");
    int n = read(fd, &buf, 12);
    
    printf("%d: %s", n, buf);

    return 0;
}
