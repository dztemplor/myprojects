#include "common.h"
int lock1(int fd)
{
     flock(fd, LOCK_EX);
}

int unlock1( int fd)
{
     flock(fd, LOCK_UN);
}


int flock_test()
{
     int fd;
     fd = open("./flock", O_CREAT, 0644);
     if (fd != -1)
     {
          lock1(fd);
          printf("after lock\n");
          sleep(20);
          unlock1(fd);
     }
     else
          perror("open");
     
}

int lock2(sem_t * p_sem)
{
     sem_wait(p_sem);
}
int unlock2(sem_t * p_sem)
{
     sem_post(p_sem);
}
/*sem_open is not easy to use, when a program exit after it acquries the sem, the sem exits in kernel and
can't be acquired by another process, you can put sem_unlink in signal handler, but if you kill -9 to the former
process, this issue is still unresolved. */
int sem_test(char * cmd)
{
     sem_t * sem;
     sem = sem_open("/sem_lock", O_CREAT, 0644, 1);
     if (sem == SEM_FAILED)
     {
          perror("sem_open");
          return -1;
     }
     
     if(!strcmp(cmd, "unlink"))
     {
          sem_unlink("/sem_lock");
          return 0;
     }

     lock2(sem);
     printf("after lock2\n");
     sleep(10);
     unlock2(sem);
}

int lock3(int sem)
{
     struct sembuf ops;
     union semun su;
     memset(&ops, 0, sizeof(ops));
     memset(&su, 0, sizeof(su));
     semctl(sem, 0, GETVAL, &su);
     printf("%s before op, get val %d\n", __func__, su.val);
     ops.sem_flg = SEM_UNDO;
     ops.sem_num = 0;
     ops.sem_op = -1;
     if(semop(sem, &ops, 1))
          perror("semop");
     semctl(sem, 0, GETVAL, &su);
     printf("%s after op, get val %d\n", __func__, su.val);

}
int unlock3(int sem)
{
     struct sembuf ops;
     memset(&ops, 0, sizeof(ops));
     ops.sem_flg = SEM_UNDO;
     ops.sem_num = 0;
     ops.sem_op = 1;
     semop(sem, &ops, 1);

}
int sem_test2(char * cmd)
{
     int sem;
     key_t key;
     int fd;
     union semun su;
     struct sembuf ops;
     fd = open("./sem1", O_CREAT, 0644);
     if (fd == -1)
          return -1;
     close(fd);
     if ( (key = ftok("./sem1", 1)) == -1)
          return -1;
     printf("%x\n", key);

     sem = semget(key, 1, 0644 | IPC_CREAT | IPC_EXCL);
     if (sem > 0 )
     {
          printf("frist creat sem\n");
          su.val = 1;
          semctl(sem, 0, SETVAL, su);
     }
     else if (sem == -1 && errno == EEXIST)
     {
          sem = semget(key, 1, 0644 |  IPC_CREAT);
          if (sem == -1)
          {
               perror("semget");
               return -1;
          }
     }
     else if (sem == -1)
          return -1;
     //we only have one sem in semset

     if (cmd && !strcmp(cmd, "get"))
     {
          su.val = 1;
          semctl(sem, 0, SETVAL, &su);
          semctl(sem, 0, GETVAL, &su);
          printf("get val %d\n", su.val);
          return -1;
     }
     else if(!strcmp(cmd, "rm"))
     {
          semctl(sem, 0, IPC_RMID, NULL);
          return -1;
     }
     lock3(sem);
     printf("after lock3\n");
     sleep(10);
     unlock3(sem);
}


int main()
{
     //flock_test();
     //sem_test("");
     sem_test2("");
}
