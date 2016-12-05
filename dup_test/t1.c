#include "common.h"



int test1()
{
     int fd;

     fd = open("./out", O_CREAT|O_RDWR, 0644);
     if (fd == -1)
     {
          perror("open");
          return -1;
     }
     if (dup2(fd, fileno(stdout))<0)
          perror("dup2");
     if (dup2(fd, fileno(stderr))<0)
          perror("dup2");

     printf("hello \n");
     printf("hello 2\n");
     write(fd, "1\n", 3);
     write(fd, "2\n", 3);
     fprintf(stderr, "stderr 1\n");
     
     if (close(fd))
          perror("close");
}

int test2()
{
     int fd, fd1;

     fd = open("./out", O_CREAT|O_RDWR, 0644);
     if (fd == -1)
     {
          perror("open");
          return -1;
     }
     fd1 = dup(fd);

     write(fd1, "1\n", 3);
     write(fd1, "2\n", 3);

     
     if (close(fd))
          perror("close");

}


int main(int argc, char * argv[])
{
     //test1();
     test2();
}
