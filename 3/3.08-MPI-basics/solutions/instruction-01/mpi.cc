#include <mpi.h>
#include <cstdio>

// To run 6 processes on the CPU;
// mpirun -host localhost -np 6 {path_to_executable_on host}

int main(int argc, char** argv) {

    // Set up MPI environment
    int ret = MPI_Init(&argc,&argv);
    if (ret != MPI_SUCCESS) {
        printf("error: could not initialize MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    // determine name, rank and total number of processes.
    int worldSize, rank, namelen;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status stat;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &namelen);

    if (rank == 0) {
      printf("Hello World from Boss process, rank %d of %d running on %s\n",
	     rank, worldSize, name);

      // Iterating through the MPI_World and waiting for the Sends. Note that
      // we skip 0 because that is the boss process.
      for(int i = 1; i < worldSize; i++) {
	// Recv functions. 
	MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &stat);
	printf ("Received Hello from rank %d!\n", rank);
      }
    } else {
      // Send functions. Note that we specify source to be rank 0, so that the
      // message is sent to the Boss worker.
      MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
}

