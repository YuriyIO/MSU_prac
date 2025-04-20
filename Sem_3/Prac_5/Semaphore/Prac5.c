#include <sys/ipc.h>
#include <sys/sem.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include<string.h>
#include<fcntl.h>

int sem;

void hander(int s)
{
    struct sembuf sem_down= {0, -1, 0};
    semop(sem, &sem_down, 1);
    printf("Please, wait the end of production.\n");
    sleep(5);
    semctl(sem, 0, IPC_RMID, 0);
    exit(0);
}

int main(int argc, char** argv)
{

    signal(SIGINT, hander);
    key_t key;

    key = ftok("C:\\Users\79304\\.vscode\\Prac5.c", 'S');
    sem = semget(key, 5, 0666 | IPC_CREAT | IPC_EXCL); 
    if(sem == -1)
    {
        exit(-1);
    }

    semctl(sem,0,SETVAL, (int) 1); // окончание производства
    semctl(sem,1,SETVAL, (int) 0); //A
    semctl(sem,2,SETVAL, (int) 0); //B
    semctl(sem,3,SETVAL, (int) 0); //C
    semctl(sem,4,SETVAL, (int) 0); //MDL

    struct sembuf down_MDL = {4, -1, 0};
    struct sembuf down_C= {3, -1, 0};

    int i = 1;
    while(1)
    {
        semop(sem, &down_MDL, 1);
        semop(sem, &down_C, 1);
        printf("Product number %d was done\n", i++);
    }

    return 0;
}