#include "stdlib.h"  
#include "string.h"  
#include "stdio.h"
 #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>
 #include <stdarg.h>





int file_printf(const char *fmt, ...)
{
    va_list ap;
    char *p;
    FILE  *fp;

    fp = fopen("/tmp/a.log", "a");
    if (!fp)
        return -1;
    
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    fclose(fp);
}
#define printf file_printf
int main(int argc, char * argv[])
{

    printf("%s %d\n", "abc", 1);
    
}
