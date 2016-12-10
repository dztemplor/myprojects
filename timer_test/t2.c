#include "common.h"

struct {
     TAILQ_HEAD(tailhead, entry) head;
     int fd1;
     int task_idx;     
}g_ctx;


                            
typedef int timer_cb(char * input)  ;

struct entry {
     TAILQ_ENTRY(entry) link;         /* Tail queue. */
     timer_cb * f;
     struct timespec ts;
     char name[128];
};

int timer_get_min_timeout(struct tailhead * head, struct timeval *p_tv)
{
     struct entry * p_e;
     struct timespec now;
     if (TAILQ_EMPTY(head))
          return -1;
     
     p_e = TAILQ_FIRST(head);
     clock_gettime(CLOCK_REALTIME, &now);
     p_tv->tv_sec = p_e->ts.tv_sec - now.tv_sec;
     if (p_tv->tv_sec < 0)
          p_tv->tv_sec = 0;
     p_tv->tv_usec = 0;
     //p_tv->tv_nsec = p_e->
     
     return 0;
}
int timer_f1(char * fn)
{
     printf("%s running\n", fn);
     return 0;
}

int timer_run_tasks(struct tailhead * head)
{
     struct entry * tmp = NULL , *np;
     struct timespec now;
     clock_gettime(CLOCK_REALTIME, &now);
     np = NULL;
     
     TAILQ_FOREACH_FROM_SAFE(np, head, link, tmp)
     {
          if (now.tv_sec >= np->ts.tv_sec)
          {
               np->f(np->name);
               TAILQ_REMOVE(head, np, link);
               free(np);
          }
     }
 
}
int add_timer_task(int i, timer_cb * func);
void * timer_loop(void * args)
{
     int fd = *((int *)args);
     fd_set rfs ; 
     struct timeval tv;
     tv.tv_sec = 0;
     tv.tv_usec = 0;
     while(1)
     {
          struct timeval * p_tv;
          int ret;
          //if head has no tasks, select wait for fifo's input
          if (timer_get_min_timeout(&g_ctx.head, &tv))
               p_tv = NULL;
          else
               p_tv = &tv;
          
          if (!p_tv)
               printf("%s block wait\n", __func__);
          FD_ZERO(&rfs);
          FD_SET(fd, &rfs);
          ret = select(fd+1, &rfs, NULL, NULL, p_tv);
          if (ret < 0 && errno == EINTR)
               continue;
          else if (ret < 0)
               return NULL;
          else if (ret > 0)
          {
               char buf[128] = {0};
               int interval;
               void * func;
               read(fd, buf, sizeof(buf));
               printf("%s buf %s\n", __func__, buf);
               if ( (ret = sscanf(buf, "add,%d,%p", &interval, &func)) == 2)
               {
                    add_timer_task(interval, func);
               }               
               else if (!strncmp(buf, "add1", 4))
               {
                    int i = 10;
                    char * p;
                    g_ctx.task_idx ++;

                    if (p = strchr(buf, ' '))
                    {
                         p++;
                         i = atoi(p) ? atoi(p):10;
                    }

                    add_timer_cmd(i);
               }
               printf("%s ret %d\n", __func__, ret);

               continue;
          }
          else if (!ret)
               timer_run_tasks(&g_ctx.head);
     }
     
     return NULL;
}

int add_timer_task(int i, timer_cb * func)
{
     struct entry * n, * np;
     struct timespec now;
     int found = 0;
     struct tailhead * head = &g_ctx.head;
     np = NULL;

     n = calloc(1, sizeof(*n));
     if (!n)
          return -1;
     snprintf(n->name, sizeof(n->name), "task%d", g_ctx.task_idx);
     
     clock_gettime(CLOCK_REALTIME, &now);
     now.tv_sec += i;
     n->ts.tv_sec =now.tv_sec;//don't consider int overflow
     n->f = func; 

     printf("%s %s %d\n", __func__, n->name, (int)n->ts.tv_sec);

     TAILQ_FOREACH(np, head, link)
     {
          if (n->ts.tv_sec < np->ts.tv_sec)
          {
               found = 1;
               TAILQ_INSERT_BEFORE(np, n, link);
          }
     }

     if (!found)
          TAILQ_INSERT_TAIL(head, n ,link);
     
     {
          char * s = "trigger\n";
          write(g_ctx.fd1, s, strlen(s));
          printf("%s need trigger\n", __func__);
     }
     
     return 0;

}
int add_timer_cmd(int i)
{
     char s[128] = {0};
     sprintf(s, "add,%d,%p", i, timer_f1);
     write(g_ctx.fd1, s, strlen(s));
     //printf("%s need trigger\n", __func__);
          
}
int del_timer_task(char * n)
{
//still need a lock, as timer thread may del this task
     struct entry * tmp = NULL , *np;
     struct tailhead * head = &g_ctx.head;
     np = NULL;
     TAILQ_FOREACH_FROM_SAFE(np, head, link, tmp)
     {
          if (!strcmp(np->name, n))
          {
               TAILQ_REMOVE(head, np, link);
               free(np);
          }
     }
     
}
void print_timer_task()
{
     struct entry * tmp = NULL , *np;
     struct tailhead * head = &g_ctx.head;
     np = NULL;
     printf("%s:\n", __func__);
     TAILQ_FOREACH_FROM_SAFE(np, head, link, tmp)
          printf("task name: %s %d\n", np->name, (int)np->ts.tv_sec);
     
}

int fifo_init(char * fn)
{
     int ret, fd ;
     remove(fn);
     ret = mkfifo(fn , 0644);
     if (ret)
     {
          perror("mkfifo");
          return -1;
     }
     
     fd = open(fn, O_RDWR);
     return fd;
}
#define FIFO_TIMER "./fifo_timer"


int test1()
{
     int ret ;
     char buf[128] = {0};
     int fd1;
     struct tailhead * head = &g_ctx.head;
     
     fd1 = fifo_init(FIFO_TIMER);
     if (fd1 <0)
          return -1;
     
     g_ctx.fd1 = fd1;

     TAILQ_INIT(head);
     pthread_t thrd;

     timer_loop(&fd1);

}

int main()
{
     test1();
}
