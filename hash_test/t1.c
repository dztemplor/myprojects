#include "common.h"


unsigned int string_hash(char *string)
{
        /* This is the djb2 string hash function */

        unsigned int result = 5381;
        unsigned char *p;

        p = (unsigned char *) string;

        while (*p != '\0') {
                result = (result << 5) + result + *p;
                ++p;
        }

        return result;
}

#define SIZE 1024
#define NUM 1024*1024
int arr[SIZE];
char alpha[26]="abcdefghijklmnopqrstuvwxyz";

char * gen_str(int len)
{
     char * str;
     str = calloc(1, len);
     if (str) 
     {
          int i;
          for (i=0; i<len-1; i++)
               str[i] = alpha[random()%26];
          return str;
     }
     return NULL;
}

int test1()
{
     char * str;
     int i;
     srandom(time(NULL));

     for (i=0; i<NUM; i++)
     {
          unsigned int h;
          str = gen_str(64);
          if (!str)
               return -1;
          h = string_hash(str);
          arr[h%SIZE]++; 
          free(str);
     }


     FILE * fp;
     fp = fopen("output", "w+");
     if (fp)
     {
          for (i=0; i<SIZE; i++)
               fprintf(fp, "%d\n", arr[i]);
          fclose(fp);
     }

          
}


int main(int argc, char * argv[])
{
     test1();
}
