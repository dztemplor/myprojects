#include "common.h"
#include <asm/types.h>
#include <linux/netlink.h>

void get_sock_pid(int fd)
{
    int ret ;
    struct sockaddr_nl tmp;

    socklen_t len = sizeof(tmp);
    ret = getsockname(fd, (struct sockaddr *) &tmp, &len);
    if (!ret)
        printf("dz %s %d\n", __func__, tmp.nl_pid);

}
int bind1()
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	struct sockaddr_nl src_addr, dest_addr;
	int sock_fd = 0;
	int ret = 0;

	sock_fd = socket(PF_NETLINK, SOCK_RAW, 15);
	if (sock_fd < 0)
    {
        perror("socket");
		goto err;
    }

	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	//src_addr.nl_pid = getpid();	/* self pid */
    src_addr.nl_pid = 0;	/* self pid */

	ret = bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));
	if (ret) {
		perror("clnt_pci_create bind fail");
		goto err;
    }
    else
        get_sock_pid(sock_fd);

err:
    return sock_fd;
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
