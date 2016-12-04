#include "common.h"

typedef struct 
{
     int mac[6];
}mac_t;
struct {
     TAILQ_HEAD(tailhead, entry) head;
     int n;
}g;

struct entry {
     TAILQ_ENTRY(entry) link;         /* Tail queue. */
     mac_t m;
} ;

int test1()
{
     FILE * fp;
     char buf[1024] = {0};
     char * tmp;
     char * str;
     struct entry * e;
     
     TAILQ_INIT(&g.head);

     fp = fopen("./input", "r");
     
     while(fgets(buf, sizeof(buf), fp))
     {
          for(str = strtok_r(buf, ",", &tmp) ;str; str =  strtok_r(NULL, ",", &tmp) )
          {
               e = calloc(1, sizeof(*e));
               
               if (sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
                          &e->m.mac[0], &e->m.mac[1], &e->m.mac[2],
                          &e->m.mac[3], &e->m.mac[4], &e->m.mac[5]) == 6)
                    g.n++;
               else
                    free(e);
               TAILQ_INSERT_TAIL(&g.head, e, link);
               printf("%s %d \n", __func__, g.n);
          }

          memset(buf, 0, sizeof(buf));
     }
     fclose(fp);
     
     e = NULL;
     struct entry * tmp1;
     TAILQ_FOREACH_FROM_SAFE(e, &g.head, link, tmp1)
     {
          printf("%02x:%02x:%02x:%02x:%02x:%02x\n", 
                 e->m.mac[0], e->m.mac[1], e->m.mac[2],
                 e->m.mac[3], e->m.mac[4], e->m.mac[5]);
          TAILQ_REMOVE(&g.head, e, link);
          free(e);
     }
}


int main(int argc, char * argv[])
{
     test1();
}
