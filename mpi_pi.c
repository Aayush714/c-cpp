#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
   MPI_Init(&argc, &argv);

   int niter = 1048576;
   double x, y;
   int i, count = 0;
   double z, pi;

   int rank, numprocs;

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

   srand(time(NULL));
   count = 0;

   double start = MPI_Wtime();

   for (i = rank; i < niter; i = i + numprocs)
   {
      x = (double)rand() / RAND_MAX;
      y = (double)rand() / RAND_MAX;
      z = x * x + y * y;
      if (z <= 1)
         count++;
   }
   double buffer;
   pi = (double)count / niter * 4;
   MPI_Reduce(&pi, &buffer, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   double end = MPI_Wtime();
   MPI_Reduce(&start, &end, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   printf("Time : %e \n", end - start);
   MPI_Finalize();
   printf("Final PI Value: %e \n", buffer);
   return 0;
}