#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <vector>


int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    double t0, tfinal;
    int size;
    int max_p = 15;
    int max_size = pow(2, max_p);
    std::vector<double> data(max_size, 0);
    int n_iter = 1000;

    for (int p = 0; p < max_p; p++)
    {
        size = pow(2, p);
        MPI_Barrier(MPI_COMM_WORLD); 

        
        t0 = MPI_Wtime();
        if (rank == 0) 
        {
            for (int iter = 0; iter < n_iter; iter++)
            {
                MPI_Send(data.data(), size, MPI_DOUBLE, 1, 1234, MPI_COMM_WORLD);
                MPI_Recv(data.data(), size, MPI_DOUBLE, 1, 4321, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        else if (rank == 1)
        {
            for (int iter = 0; iter < n_iter; iter++)
            {
                MPI_Recv(data.data(), size, MPI_DOUBLE, 0, 1234, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data.data(), size, MPI_DOUBLE, 0, 4321, MPI_COMM_WORLD);
            }
        }
        tfinal = (MPI_Wtime() - t0)/ n_iter;
        MPI_Reduce(&tfinal, &t0, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        if (rank == 0) printf("Size %d, PingPong %e\n", size, t0);
    }

    MPI_Finalize();
    return 0;
}
