#include <cstdlib>
#include <iostream>
#include <ctime>
#include <mpi.h>

int main ( int argc, char *argv[] )
{
  int myrank, numprocs;
  double wtime;
//
//  Initialize MPI.
//
  MPI::Init ( argc, argv );
//
//  Get the number of processes.
//
  numprocs = MPI::COMM_WORLD.Get_size ( );
//
//  Get the individual process ID.
//
  myrank = MPI::COMM_WORLD.Get_rank ( );
//
//  Process 0 prints an introductory message.
//
  if ( myrank == 0 ) 
  {
    std::cout << "\n";
    std::cout << "HELLOMPI - Master process:\n";
    std::cout << "  C++/MPI version\n";
    std::cout << "  An MPI example program.\n";
    std::cout << "\n";
    std::cout << "  The number of processes is " << numprocs << "\n";
    std::cout << "\n";
  }
//
//  Every process prints a hello.
//
  if ( myrank == 0 ) 
  {
    wtime = MPI::Wtime ( );
  }
  std::cout << "  Process " << myrank << " says 'Hello, world!'\n";
//
//  Terminate MPI.
//
  MPI::Finalize ( );
//
//  Process 0 says goodbye.
//
  if ( myrank == 0 )
  {
    wtime = MPI::Wtime ( ) - wtime;
    std::cout << "  Elapsed wall clock time = " << wtime << " seconds.\n";
  }
//
//  Terminate.
//
  if ( myrank == 0 )
  {
    std::cout << "\n";
    std::cout << "HELLOMPI:\n";
    std::cout << "  Normal end of execution.\n";
    std::cout << "\n";
  }
  return 0;
}
