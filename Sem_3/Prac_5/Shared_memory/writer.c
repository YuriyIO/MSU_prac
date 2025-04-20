#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char** argv)
{
    key_t key;
    int sem;
    int shm;
    char* shmaddr;

    key = ftok("C:\\Users\79304\\.vscode\\writer.c", 'S');
    sem = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL); 
    if(sem == -1)
    {
        exit(-1);
    }
    shm = shmget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if(shm == -1)
    {
        exit(-1);
    }
    shmaddr = shmat(shm, NULL, 0);
    if(shmaddr == NULL)
    {
        exit(-1);
    }

    semctl(sem,0,SETVAL, (int) 0); //writer
    semctl(sem,1,SETVAL, (int) 0); //reader

    struct sembuf down_wr = {0, -1, 0};
    struct sembuf up_rd= {1, 1, 0};

    printf("Start writing:\n");
    int c;
    while(1)
    {  
        c = getchar();
        shmaddr[0] = c;
        semop(sem, &up_rd, 1);
        semop(sem, &down_wr, 1);
        if(c == EOF)
        {
            printf("End writing.\n");
            break;
        }
    }



    shmctl(shm, IPC_RMID, NULL); 
    semctl(sem, 0, IPC_RMID, 0);
    return 0;
}