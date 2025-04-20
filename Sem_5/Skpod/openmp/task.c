#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define  Max(a,b) ((a)>(b)?(a):(b))

#define  N      (2*2*2*2*2*2+2)
float   maxeps = 0.1e-7;
int itmax = 100;
float eps;

float A [N][N][N];

void relax();
void init();
void verify(); 


int main(int an, char **as)
{
    printf("N == %d\n", N);
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 40, 60, 80, 100, 120, 140, 160};
	int len = 18;
	int tmp;
	for(tmp = 0; tmp < len; tmp++)
	{
		int p = arr[tmp];
		printf("p == %d\n", p);
		int it;
		float start = omp_get_wtime();

    	omp_set_num_threads(p);

		init();
		for(it=1; it<=itmax; it++)
		{
			eps = 0.;
			relax();
			if (eps < maxeps) break;
            printf("%d %f\n", it, eps);
		}

		verify();

		float fin = omp_get_wtime();
		printf("runtime = %f\n",fin - start);
	}


	return 0;
}


void f1(int i)
{
    int k, j;
    for(k=0; k<=N-1; k++)
	for(j=0; j<=N-1; j++)
	{
		if(i==0 || i==N-1 || j==0 || j==N-1 || k==0 || k==N-1)
		    A[i][j][k]= 0.;
		else A[i][j][k]= ( 4. + i + j + k) ;
	}
}

void init()
{
    #pragma omp parallel
    {
        int i;
        #pragma omp single
        {
        for(i=0; i<=N-1; i++)
            #pragma omp task
                f1(i);
        }
    }
}

void f21(int num, int i)
{
    int j, k;
    for(j = 1; j <= num + 1 - i; j++)
    {
        k = num + 2 - i - j;
        float e = A[i][j][k];
		A[i][j][k]=(A[i-1][j][k]+A[i+1][j][k]+A[i][j-1][k]+A[i][j+1][k]+A[i][j][k-1]+A[i][j][k+1])/6.;
		eps=Max(eps, fabs(e-A[i][j][k]));
    }
}


void f22(int num, int i)
{
    int j, k;
    for(j = Max(num + 2 - i - N + 2, 1); j <=  N - 2; j++)
    {
        k = num + 2 - i - j;
        if(k < 1)
            break;
        float e = A[i][j][k];
		A[i][j][k]=(A[i-1][j][k]+A[i+1][j][k]+A[i][j-1][k]+A[i][j+1][k]+A[i][j][k-1]+A[i][j][k+1])/6.;
		eps=Max(eps, fabs(e-A[i][j][k]));
    }
}


void f23(int num, int i)
{
    int j, k;
    for(j = Max(num + 2 - i - N + 2, 1); j <= N - 2; j++)
    {
        k = num + 2 - i - j;
        float e = A[i][j][k];
		A[i][j][k]=(A[i-1][j][k]+A[i+1][j][k]+A[i][j-1][k]+A[i][j+1][k]+A[i][j][k-1]+A[i][j][k+1])/6.;
		eps=Max(eps, fabs(e-A[i][j][k]));
    }
}

void relax()
{
    int num;

    for(num = 1; num <= (N - 2); num++)
    {
    #pragma omp parallel shared(A) reduction(max: eps)
    {
        int i;
        #pragma omp single
        {
        for(i = num; i >= 1; i--)
            #pragma omp task
                f21(num, i);
        }
    }
    }

    for(num = N - 1; num <= 2 * (N - 2) - 2; num++)
    {
    #pragma omp parallel shared(A) reduction(max: eps)
    {
        int i;
        #pragma omp single
        {
        for(i = N - 2; i >= 1; i--)
            #pragma omp task
                f22(num, i);
        }   
    }
    
    }

    for(num =  2 * (N - 2) - 1; num <= 3 * (N - 2) - 2; num++)
    {
    #pragma omp parallel shared(A) reduction(max: eps)
    {
        int i;
        #pragma omp single
        {
        for(i = N-2; i >= 1; i--)
            #pragma omp task
                f23(num, i);
        }
    }
    }
}


float s=0.;
void f3(int i)
{
    int k, j;
    for(k=0; k<=N-1; k++)
	for(j=0; j<=N-1; j++)
	{
		s += A[i][j][k]*(i+1)*(j+1)*(k+1)/(N*N*N);
    }
    
}

void verify()
{ 
    #pragma omp parallel shared(A) reduction(+: s)
    {
        int i;
        #pragma omp single 
        {
            for(i=0; i<=N-1; i++)
            #pragma omp task
                f3(i);
        }
    }

	printf("S = %f\n",s);
}
