#include "common.h"


#define FIFO "./fifo"

sem_t sem_sch;
char evt[128];


void * scheduler(void * args)
{
     while(!sem_wait(&sem_sch))
     {
          
     }



     return NULL;
}
// TAILQ_HEAD(tailhead, entry) head;
void * thrd_run(void * args)
{
     char * str = (char * )args;

     printf("thrd_run: %s\n", str);
     int fd ;
     fd = open(FIFO, O_RDWR);
     if (fd<0)
          return NULL;
     
     
     sleep(5);

     char buf[128] = {0};
     snprintf(buf, sizeof(buf)-1, "end:%s", str);
     write(fd, buf, strlen(buf));
     
     free(str);

     printf("end of thrd_run\n");
     return NULL;
}
typedef enum {
     RUNNING,
     PENDING
}state_t;


TAILQ_HEAD(tailhead, entry) head;
struct entry {
           TAILQ_ENTRY(entry) entries;         /* Tail queue. */
     state_t state;
     char e[128]; //event
};
     



int main()
{

     
     int ret, fd ;
     remove(FIFO);
     ret = mkfifo(FIFO , 0644);
     if (ret)
     {
          perror("mkfifo");
          return -1;
     }
     
     fd = open(FIFO, O_RDWR);
     if (fd <0)
          return -1;
     
     sem_init(&sem_sch, 0, 0);
 
     TAILQ_INIT(&head);
     char buf[128] ={0};
     pthread_t thrd;
     while( read(fd, buf, sizeof(buf)-1))
     {
          printf("buf %s\n", buf);
          
          if (!strncmp(buf, "evt:", 4))
          {
               char * str = NULL;
               str = strchr(buf, ':');
               str++;
               
               struct entry * np, *new;
               for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next)
               {
                    if (!strcmp(np->e, str))
                         break;
               }

                new = calloc(1, sizeof(*new));
                strncpy(new->e,  str, sizeof(new->e));

               if (np)
               {
                    //same typed event is running, add the new one as pending
                    new->state = PENDING;
                    TAILQ_INSERT_TAIL(&head, new, entries);
               }
               else 
               {
                    char * evt ;
                    evt = calloc(1,128);
                    strcpy(evt, str);
                    pthread_create(&thrd, NULL, thrd_run, evt);
                    pthread_detach(thrd);
                    
                    new->state = RUNNING;
                    TAILQ_INSERT_TAIL(&head, new, entries);
               }
          }
          else if (!strncmp(buf, "end:", 4))
          {
               char * str = NULL;
               str = strchr(buf, ':');
               str++;

               struct entry * np, *new;
               
               //remove the running one
               for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next)
               { 
                    if (!strcmp(np->e, str) && np->state == RUNNING)
                         break;
               }
               if (np)
               {
                    TAILQ_REMOVE(&head, np, entries);
                    free(np);
               }
         
               //run the pending one
               for (np = head.tqh_first; np != NULL; np = np->entries.tqe_next)
               { 
                    if (!strcmp(np->e, str) && np->state == PENDING)
                         break;
               }
               if (np)
               {
                    char * evt ;
                    evt = calloc(1,128);
                    strcpy(evt, str);
                    pthread_create(&thrd, NULL, thrd_run, evt);
                    pthread_detach(thrd);

                    np->state = RUNNING;
               }
               
           }
          
     }
}
