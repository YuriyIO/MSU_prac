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

int main(int argc, char** argv)
{
    key_t key;
    int sem;

    key = ftok("C:\\Users\79304\\.vscode\\Prac5.c", 'S');
    sem = semget(key, 0, 0666);
    if(sem == -1)
    {
        exit(1);
    }

    struct sembuf up_B = {2, 1, 0};
    while(semctl(sem, 0, GETVAL, 0))
    {
        sleep(3);
        semop(sem, &up_B, 1);
    }

    return 0;
}