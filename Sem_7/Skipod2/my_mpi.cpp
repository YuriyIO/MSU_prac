#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <signal.h>
#include <mpi-ext.h>

using namespace std;

//mpicc my_mpi.c -o my_mpi -Wall -ggdb3 
//mpirun --oversubscribe -n 2 my_mpi
//mpirun -n 4 --with-ft ulfm mt
//mpirun -n 5 --with-ft ulfm my_mpi

#define N (2*2*2*2*2*2+2)
//#define N 10

#define KILLED_PROCESS 2
#define DEAD_IT 40

//#define A_FLOC(i,j,k) (A[(i)-startrow+1][j][k])
//#define B_FLOC(i,j,k) (B[(i)-startrow][j][k])
//S = 2814659.450575

#define m_printf if(my_rank==0)printf

double maxeps = 0.1e-7;
int itmax = 100;
int i,j,k;
double eps;
double sum;

struct two_row
{
    double data[N][N];
};

two_row* A;
two_row* B;



MPI_Request request[4];
MPI_Status status[4];
MPI_Comm main_comm;

int startrow, lastrow;
int n_rows;
int my_size;
int my_rank;
int len;
int save_iter = 0;


void relax();
void init();
void verify();
double*** get_array(int len);
void free_array(double**** t, int len);
double*** allocate3D(int l,int m,int n);
void deallocate3D(double*** arr3D, int len);
void write_to_file();
void read_from_file();
void reset_param();



static void verbose_errhandler(MPI_Comm* pcomm, int* perr, ...)
{
    int err = *perr;
    char errstr[MPI_MAX_ERROR_STRING];
    int i, nf, len, eclass;
    MPI_Group group_c, group_f;
    int *ranks_gc, *ranks_gf;

    MPI_Error_class(err, &eclass);
    if( MPIX_ERR_PROC_FAILED != eclass ) {
        MPI_Abort(main_comm, err);
    }
    MPI_Comm_rank(main_comm, &my_rank);
    MPI_Comm_size(main_comm, &my_size);
    /* We use a combination of 'ack/get_acked' to obtain the list of failed processes.     */
    MPIX_Comm_failure_ack(main_comm);
    MPIX_Comm_failure_get_acked(main_comm, &group_f);
    MPI_Group_size(group_f, &nf);
    MPI_Error_string(err, errstr, &len);
    printf("Rank %d / %d: Notified of error %s. %d found dead: { ", my_rank, my_size, errstr, nf);
    /* We use 'translate_ranks' to obtain the ranks of failed procs  in 'comm‘ communicator  */
    ranks_gf = (int*)malloc(nf * sizeof(int));
    ranks_gc = (int*)malloc(nf * sizeof(int));
    MPI_Comm_group(main_comm, &group_c);
    for(i = 0; i < nf; i++)
        ranks_gf[i] = i;
    MPI_Group_translate_ranks(group_f, nf, ranks_gf,
                              group_c, ranks_gc);
    for(i = 0; i < nf; i++)
        printf("%d ", ranks_gc[i]);
    printf("}\n");
    free(ranks_gf); free(ranks_gc);


    MPIX_Comm_shrink(main_comm, &main_comm);
    MPI_Comm_size(main_comm, &my_size);
    MPI_Comm_rank(main_comm, &my_rank);

    throw("one death");
}

int main(int argc, char **argv)
{
    main_comm = MPI_COMM_WORLD;
    double time_start, time_fin;
    int it;
    MPI_Errhandler errh;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(main_comm, &my_rank);
    MPI_Comm_size(main_comm, &my_size);
    MPI_Comm_create_errhandler(verbose_errhandler, &errh);
    MPI_Comm_set_errhandler(main_comm, errh);
    MPI_Barrier(main_comm);  
    
	time_start = MPI_Wtime();

	m_printf("N %d\n", N);
    m_printf("P %d\n", my_size);


    startrow = (my_rank * N) / my_size;        
    lastrow = ((my_rank + 1) * N) / my_size - 1;
    n_rows = lastrow - startrow + 1;    
    int stop = 0;


    if(startrow == 0 || lastrow == N-1)
        len = n_rows + 1;
    else
        len = n_rows + 2;

    A = new two_row[len];
    B = new two_row[n_rows];


	init();
    MPI_Barrier(main_comm);

    write_to_file();
    save_iter = 1;
    int fst_kill = 1;
    for(it=1; it<=itmax; it++)
	{
       try
       {
            if(it%10 == 0)
            {
                write_to_file();
                save_iter = it;
            }
            if(it == DEAD_IT && fst_kill == 1)
            {
                printf("dead\n");
                fst_kill = 0;
                if(my_rank == KILLED_PROCESS) raise(SIGKILL);
                MPI_Barrier(main_comm);
            }
            eps = 1;
            relax();
            m_printf("it=%4i   eps=%f\n", it, eps);
            if(eps < maxeps) {
                stop = 1;
            }
            MPI_Bcast(&stop, 1, MPI_INT, 0, main_comm);
            if(stop) {
                break;
            }
       }
       catch(char const* e)
       {
            reset_param();
            read_from_file();
            it = save_iter-1;
       }
	}

    verify();
	time_fin = MPI_Wtime();
    printf("runtime == %d: %lf\n", my_rank, time_fin - time_start);

    free(A);
    free(B);


    MPI_Barrier(main_comm);
    MPI_Finalize();
	return 0;
}




void init()
{
    int add = startrow-1;
    if(startrow == 0)
        add = 0;
    for(i=0; i < len; i++)
	for(j=0; j<=N-1; j++)
	for(k=0; k<=N-1; k++)
	{
        A[i].data[j][k] = 1;
		if((i+add) == 0|| (i+add) == N-1 || j==0 || j==N-1 || k==0 || k==N-1)
            A[i].data[j][k] = 0.;
		else
			A[i].data[j][k] = 4.0 + (i+add) + j + k;
	}
}

void relax()
{
    double e;
    double loceps = eps;

    for(i=1; i <= len-2; i++)
    for(j=1; j<=N-2; j++)
    for(k=1; k<=N-2; k++)
    {
        B[i-1].data[j][k]= (A[i-1].data[j][k]+A[i+1].data[j][k]
					+A[i].data[j-1][k]+A[i].data[j+1][k]
					+A[i].data[j][k-1]+A[i].data[j][k+1])/6.;
    }

    for(i=1; i <= len-2; i++)
    for(j=1; j<=N-2; j++)
    for(k=1; k<=N-2; k++)
    {                
        e = fabs(B[i-1].data[j][k] - A[i].data[j][k]);
        if(e > loceps) 
                loceps = e;
        A[i].data[j][k] = B[i-1].data[j][k];     
    }


    if(startrow != 0)
        MPI_Irecv(&A[0], N*N, MPI_DOUBLE, my_rank-1, 1235,
            main_comm, &request[0]);
    if(lastrow != N - 1)
        MPI_Isend(&B[len-3], N*N, MPI_DOUBLE, my_rank+1, 1235,
            main_comm, &request[2]);
    if(lastrow != N - 1)
        MPI_Irecv(&A[len-1], N*N, MPI_DOUBLE, my_rank+1, 1236,
            main_comm, &request[3]);
    if(startrow  != 0)
        MPI_Isend(&B[0], N*N, MPI_DOUBLE, my_rank-1, 1236,
            main_comm, &request[1]);

    int ll = 4;
    int shift = 0;
    if(startrow == 0)
    {
        ll = 2;
        shift = 2;
    }
    if(lastrow == N - 1)
        ll = 2;
    MPI_Waitall(ll, &request[shift], &status[0]);
    MPI_Barrier(main_comm);
    MPI_Reduce(&loceps, &eps, 1, MPI_DOUBLE, MPI_MAX, 0, main_comm);
}

void verify()
{
    double s = 0.;
    int st = 1;
    int fn = len-2;
    int add = startrow;

    if(startrow == 0)
    {
        st = 0;
        add = 1;
    }
    if(lastrow == N - 1)
        fn = len-1;

    for(i=st; i <=fn; i++)
	for(j=0; j<=N-1; j++)
	for(k=0; k<=N-1; k++)
	{
		    s += A[i].data[j][k]*(i+add)*(j+1)*(k+1)/(N*N*N);
	}


    MPI_Barrier(main_comm);
    MPI_Reduce(&s, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, main_comm);
	m_printf("  S = %f\n", sum);
}


void write_to_file()
{
    MPI_File fh;
    int st = 1;
    if(startrow == 0)
    {
        st = 0;
    }

    
    MPI_File_open(main_comm, "my_file",
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, startrow * sizeof(two_row), MPI_SEEK_SET);
    MPI_File_write_all(fh, &(A[st]), n_rows*N*N, MPI_DOUBLE,
                   MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
}

void read_from_file()
{
    MPI_File fh;
    MPI_Status status;
    int j;

    int count = startrow-1;
    if(startrow == 0)
    {
        count = 0;
    }

    MPI_File_open(main_comm, "my_file",
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, count *sizeof(two_row), MPI_SEEK_SET);
    MPI_File_read_all(fh, A, len*N*N, MPI_DOUBLE, &status);
    MPI_File_close(&fh); 
}

void reset_param()
{
    free(A);
    free(B);

    startrow = (my_rank * N) / my_size;        
    lastrow = ((my_rank + 1) * N) / my_size - 1;
    n_rows = lastrow - startrow + 1;    

    if(my_rank == 0 || my_rank == my_size-1)
        len = n_rows + 1;
    else
        len = n_rows + 2;
    

    A = new two_row[len];
    B = new two_row[n_rows];
}