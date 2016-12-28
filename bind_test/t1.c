#include "common.h"

int bind1()
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
    int ret;

#if 1
	listenfd = socket(AF_INET, SOCK_STREAM|SOCK_CLOEXEC, 0);
#else
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
#endif
    if (listenfd <0)
        perror("socket");
    {
        int on=1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    }
    
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(10000);

	ret = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (ret)
        perror("bind");
    
    listen(listenfd, 5);
    return listenfd;

}
void t2()
{
    int fd;
    pid_t pid;
    printf("t2 1\n");
    fd = bind1();
    if ((pid = fork()) == 0)
    {
        //close(fd);

        if (execl("/bin/sh", "sh", "-c", "/home/zdu/test_files/a.sh", (char *)0))
            perror("exec");
        exit(0);
    }
    close(fd);
    printf("t2 2\n");
    sleep(2);
    fd = bind1();
    exit(0);
}

int main(int argc, char * argv[])
{
    t2();

}
