#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>


const int N = 36;

#define SEND_ARRAY 1
#define SEND_RANK 2
#define SEND_FINAL 3

bool try_luck()
{
    return (RAND_MAX/2 > rand()) ;
}

char* get_pos(int n)
{
    switch(n)
    {
        case 0: return "(0,0)"; case 1: return "(0,1)";
        case 2: return "(0,2)"; case 3: return "(0,3)";
        case 4: return "(0,4)"; case 5: return "(0,5)";
        case 6: return "(1,5)"; case 7: return "(1,4)";
        case 8: return "(1,3)"; case 9: return "(1,2)";
        case 10: return "(1,1)"; case 11: return "(2,1)";
        case 12: return "(2,2)"; case 13: return "(2,3)";
        case 14: return "(2,4)"; case 15: return "(2,5)"; 
        case 16: return "(3,5)"; case 17: return "(3,4)";
        case 18: return "(3,3)"; case 19: return "(3,2)"; 
        case 20: return "(3,1)"; case 21: return "(4,1)"; 
        case 22: return "(4,2)"; case 23: return "(4,3)";
        case 24: return "(4,4)"; case 25: return "(4,5)"; 
        case 26: return "(5,5)"; case 27: return "(5,4)"; 
        case 28: return "(5,3)"; case 29: return "(5,2)";
        case 30: return "(5,1)"; case 31: return "(5,0)"; 
        case 32: return "(4,0)"; case 33: return "(3,0)"; 
        case 34: return "(2,0)"; case 35: return "(1,0)";
    }
    return "error";
}



void my_wait()
{
    double start = MPI_Wtime();
    while(MPI_Wtime()-start<0.5)
        wait(NULL);
}


int main(int argc, char* argv[]) 
{
    int size;
    int rank;


    MPI_Init(&argc, &argv);               
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //проверяем введеные значения
    int the_chosen_one = 4;
    if (argc == 2)
        the_chosen_one = atoi(argv[1]);

    if (size != N || the_chosen_one >= N) 
    {
        if (rank == 0) 
        {
            if (size != N) 
                printf("36 processes were expected\n");
            if (the_chosen_one >= N) 
                printf("coordinator must be less then 36\n");
        }
        MPI_Finalize();
        return 0;
    }

    //убиваем случайные процессы, уроме выбранного
    srand(time(NULL)*rank);
    bool alive;

    if (rank == the_chosen_one)
        alive = true;
    else
        alive = try_luck();
    
    //выводим номера живых процессов
    if (rank == the_chosen_one)
        printf("alive:\n");
    MPI_Barrier(MPI_COMM_WORLD);

    if (alive)
        printf("%d ", rank);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == the_chosen_one)
        printf("\n");

    //убиваем невыбранные процессы
    if (alive)
    {
    //создаем массив для определения живых процессов
    //в последнем элемента храним номер текущего
    int* array = (int*)malloc((size+1)*sizeof(int));
    for (int i = 0; i < size; ++i)
        array[i] = 0;

    array[size] = rank;


    //начинаем поиск живых процессов
    MPI_Request my_send_array, my_send_rank, my_recv_array, my_recv_rank;
    MPI_Status status;


    int send_size = size+1;
    
    //начинаем с выбранного процесса
    if(rank == the_chosen_one)
    {
        printf("\nStart making an array of alive procc\n");
        fflush(stdout);
        array[rank] = 1;
        int send_flag = 0, check;
        int next = (rank+1)%size;

        while(!send_flag)
        {
            printf("%s send to %s \n", get_pos(rank), get_pos(next));
            fflush(stdout);

            //отправляем текущий массив
            MPI_Isend(array, send_size, MPI_INT, next, SEND_ARRAY, MPI_COMM_WORLD, &my_send_array);
            //ждем ответа, для подтверждения отправки(т.е. проверяем, что отправили живому процессу)
            MPI_Irecv(&check, 1, MPI_INT, MPI_ANY_SOURCE, SEND_RANK, MPI_COMM_WORLD, &my_recv_rank);

            my_wait();
            MPI_Test(&my_recv_rank, &send_flag, &status);
            my_wait();


            //не отправили => отправляем следующему, иначе заканчиваем
            if(!send_flag)
            {
                printf("%s has no answer from %s\n",get_pos(rank), get_pos(next));
                fflush(stdout);
                MPI_Cancel(&my_recv_rank);
                MPI_Request_free(&my_recv_rank);
            }
            else
                break;
            
            //номер следующего
            next = (next+1)%size;
            //прошли круг => живы только мы => мы координатор
            if(next == the_chosen_one)
            {
                MPI_Isend(array, size, MPI_INT, next, SEND_ARRAY, MPI_COMM_WORLD, &my_send_array);
                break;
            }
        }

        MPI_Irecv(array, size, MPI_INT, MPI_ANY_SOURCE, SEND_ARRAY, MPI_COMM_WORLD, &my_recv_array);
        MPI_Wait(&my_recv_array, &status);
        printf("\nfinish\n");
        fflush(stdout);
    }
    else
    {
        //получаем массив
        MPI_Irecv(array, send_size, MPI_INT, MPI_ANY_SOURCE, SEND_ARRAY, MPI_COMM_WORLD, &my_recv_array);
        MPI_Wait(&my_recv_array, &status);
        //номер отправителя
        int last_procc = array[size];

        printf("%s get array from %s \n", get_pos(rank), get_pos(last_procc));
        fflush(stdout);
        //подтверждаем получение
        MPI_Isend(&rank, 1, MPI_INT, last_procc, SEND_RANK, MPI_COMM_WORLD, &my_send_rank);


        array[rank] = 1;
        //восстанавливаем данные
        array[size] = rank;

        int send_flag = 0, check;
        int next = (rank+1)%size;

        while(!send_flag)
        {
            //последняя отправка
            if(next == the_chosen_one)
            {
                
                printf("%s send to %s \n", get_pos(rank), get_pos(next));
                fflush(stdout);
                MPI_Isend(array, size, MPI_INT, next, 1, MPI_COMM_WORLD, &my_send_array);
                break;
            }
            printf("%s send to %s \n", get_pos(rank), get_pos(next));
            fflush(stdout);

            //отправляем обновленный массив
            MPI_Isend(array, send_size, MPI_INT, next, SEND_ARRAY, MPI_COMM_WORLD, &my_send_array);
            //ждем ответа
            MPI_Irecv(&check, 1, MPI_INT, MPI_ANY_SOURCE, SEND_RANK, MPI_COMM_WORLD, &my_recv_rank);

            my_wait();
            MPI_Test(&my_recv_rank, &send_flag, &status);
            my_wait();

            //не отправили => отправляем следующему, иначе заканчиваем    
            if(!send_flag)
            {
                printf("%s has no answer from %s\n",get_pos(rank), get_pos(next));
                fflush(stdout);
                MPI_Cancel(&my_recv_rank);
                MPI_Request_free(&my_recv_rank);
            }
            else
                break;

            next = (next+1)%size;
        }
    }

    //координатор передает совй номер и массив живых процессов другим
    if(rank == the_chosen_one)
    {
        for(int i = 0; i < size; ++i)
            if(array[i])
                MPI_Isend(array, send_size, MPI_INT, i, SEND_FINAL, MPI_COMM_WORLD, &my_send_array);
    }
    else
    {
        MPI_Irecv(array, send_size, MPI_INT, MPI_ANY_SOURCE, SEND_FINAL, MPI_COMM_WORLD, &my_recv_array);
        MPI_Wait(&my_recv_array, &status);
        printf("%s new coord: %s\n", get_pos(rank), get_pos(array[size]));
        fflush(stdout);

    }
    free(array);
    }
    
    //ждем все процессы
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}