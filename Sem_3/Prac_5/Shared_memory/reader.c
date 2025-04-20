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
    sem = semget(key, 2, 0666); 
    if(sem == -1)
    {
        exit(-1);
    }
    shm = shmget(key, 1, 0666);
    if(shm == -1)
    {
        exit(-1);
    }
    shmaddr = shmat(shm, NULL, 0);
    if(shmaddr == NULL)
    {
        exit(-1);
    }

    struct sembuf up_wr = {0, 1, 0};
    struct sembuf down_rd= {1, -1, 0};

    printf("Start reading:\n");
    int c;
    while(1)
    {   
        semop(sem, &down_rd, 1);
        c = shmaddr[0];
        printf("%c", c);
        semop(sem, &up_wr, 1);
        if(c == EOF)
        {
            printf("End reading.\n");
            break;
        }
    }


    return 0;
}