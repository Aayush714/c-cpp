#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

// Function for Binomial Algorithm 
int bino_algo(int rank,int nprocs, int arraySize, double* rBuffer, double* sBuffer)
{
    int new = 0;
    int tag = 9999;
    int left = nprocs;
	
    MPI_Status receive;

    // Running the loop for log n where n is number of processes 
	for(int i = 0; i < log2(nprocs) ; i++)
    {
        //identifying the next stride
		new = left / 2;

        // if the current rank is one of the sending nodes 
		if( rank % left == 0 )
			MPI_Send( sBuffer, arraySize, MPI_DOUBLE, rank+new, tag, MPI_COMM_WORLD); // msg sending by sending nodes 
		
        // For the other nodes to receive the msg from sending nodes 
        else if ( rank % ( left / 2 ) == 0 )
        {
			MPI_Recv( rBuffer, arraySize, MPI_DOUBLE, rank-new, tag, MPI_COMM_WORLD, &receive);
			sBuffer = rBuffer; //assigning the receiving buffer to sending buffer 
		}	
		
        // Identifying the left nodes which will be including in the sending nodes 
        left = left / 2;
	}

}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
	
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size, rank, num_procs;

    double *msg; // the message which will be sent 
    double *received_msg; // the received message 
    double start, end;
	
    //looping 2^25 message sizes 
    for(int i = 0 ; i <= 25 ; i++)
    {
        size = 2**i ; // changing the size of the message 
     
        msg = (double*) malloc(size*sizeof(double*)); // assigning the new message size 
		received_msg = (double*) malloc(size*sizeof(double*)); // assigning the size to the received message variable 
		
        // looping through the current message size 
        for(int j = 0 ; j < size ; j++)
        {
            //assigning the received message as 0 
        	received_msg[j] = 0;

            // process 0 assigning the message as j 
    		if( rank == 0)
				msg[j] = j;
		
        }
		
		MPI_Barrier(MPI_COMM_WORLD);
		
        //calculating the time complexity 
        start = MPI_Wtime();
		
        //calling the binomial algorithm function 
        bino_algo(rank,num_procs,size,received_msg,msg);	
		
        end = MPI_Wtime() - start;
		
        if( rank == 0 )
			printf("%e \n",end);
		
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}