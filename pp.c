#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  int rank, size, exp;
  int iter = 1000;
  int tag = 1111;
  MPI_Status recv_status;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(NULL)*rank);
  for(int k = 0; k <= 15; k++)
  {
    exp = k;
    size = 1;
    while(exp != 0)
    {
      size *= 2;
      exp --;
      //printf("size = %d, exp = %d", size, exp);
    }
    //printf("size = %d or 2^%d\n", size, k);
    double* var = (double*)malloc(size*sizeof(double));
    for(int i = 0; i < size; i++)
    {
      var[i] = rand();
    }

    //if(rank == 0) printf("Rank 0 Initial Value = %f\n", var[0]);
    //if(rank == 1) printf("Rank 1 Initial Value = %f\n", var[0]);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    for(int i = 0; i < iter; i++)
    {
      if(rank==0)
      {
         MPI_Send(var, size, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD);
         MPI_Recv(var, size, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, &recv_status);
      }
      if(rank==1)
      {
         MPI_Recv(var, size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &recv_status);
         //printf("Rank 1 First Send = %f\n", var[0]);
        // var[0] += 10000000; //changes the message before sending back to ensure the send back is working correctly
         MPI_Send(var, size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    double finalTime = (MPI_Wtime() - start) / iter;
    //if(rank == 1) printf("Rank 1 Final Send = %f\n", var[0]);
    if(rank == 0)
    {
      //printf("Rank 0 Final Send = %f\n", var[0]);
      printf("[%e]\n", finalTime);
      //printf("size = %d or 2^%d\n", size, k);
    }
  }
  MPI_Finalize();
  return 0;
}