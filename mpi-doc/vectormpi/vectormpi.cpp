#include <cstdlib>
#include <iostream>
#include <ctime>
#include <mpi.h>
#include <vector>

int main ( int argc, char *argv[] )
{
  int myrank, numprocs;
  double wtime;
  // Initialize MPI.
  MPI::Init ( argc, argv );
  // Get the number of processes.
  numprocs = MPI::COMM_WORLD.Get_size();
  // Get the individual process ID.
  myrank = MPI::COMM_WORLD.Get_rank();
  // Process 0 prints an introductory message.
  std::vector<double> data;

  if ( myrank == 0 ) 
  {
    std::cout << "  The number of processes is " << numprocs << "\n";

    // initialize data
    for (int i=0;i<5;i++) data.push_back((double)i/2);
    // prepare send
    int size = data.size();
    // send to every other process
    for (int i=1;i<numprocs;i++) {
      std::cout << "Sending to process " << i << std::endl;
      MPI::COMM_WORLD.Send(&size, 1, MPI::INT, i, 1);
      MPI::COMM_WORLD.Send(&data.front(), size, MPI::DOUBLE, i, 2);
    }
  } else {
    std::cout << "  Process " << myrank << " receives ...'\n";
    int size;
    // recive vector size
    MPI::COMM_WORLD.Recv(&size, 1, MPI::INT, 0, 1);
    // resize vector
    data.resize(size);
    // recive vector data
    MPI::COMM_WORLD.Recv(&data.front(), size, MPI::DOUBLE, 0, 2);
    std::cout << "Process " << myrank << " got a vector of " << size << std::endl;
  }

  // Every process prints its data
  for (int i=0;i<data.size();i++)
    std::cout << myrank << " "<< data[i] << std::endl;
  // Terminate MPI.
  MPI::Finalize ( );
  // Process 0 says goodbye.
  if ( myrank == 0 )
  {
    wtime = MPI::Wtime ( ) - wtime;
    std::cout << "  Elapsed wall clock time = " << wtime << " seconds.\n";
  }
  //  Terminate.
  if ( myrank == 0 )
  {
    std::cout << "\n";
    std::cout << "VECTORMPI:\n";
    std::cout << "  Normal end of execution.\n";
    std::cout << "\n";
  }
  return 0;
}

// vim:set et sts=2 ts=2 sw=2 ai ci cin:
