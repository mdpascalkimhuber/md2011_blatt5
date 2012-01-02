#include <cstdlib>
#include <iostream>
#include <ctime>
#include <mpi.h>
#include <vector>

#define DIM 3

class Particle {
public:
    int id;
    double m;
    double x[DIM];
    double v[DIM];
};

int main ( int argc, char *argv[] )
{
  int myrank, numprocs;
  double wtime = MPI::Wtime();
  // Initialize MPI.
  MPI::Init ( argc, argv );
  // Get the number of processes.
  numprocs = MPI::COMM_WORLD.Get_size();
  // Get the individual process ID.
  myrank = MPI::COMM_WORLD.Get_rank();

  // Initialize Particle
  Particle p;

  p.id = 0;
  p.m = 0.0;
  p.x[0] = p.x[1] = p.x[2] = 0.0;
  p.v[0] = p.v[1] = p.v[2] = 0.0;

  // build new mpi datatype
  MPI::Datatype MPI_Particle;
  MPI::Datatype type[4] = {MPI::INT, MPI::DOUBLE, MPI::DOUBLE, MPI::DOUBLE};
  int blocklen[4] = {1,1,DIM,DIM};
  MPI::Aint disp[4]; // displacements
  mpi::Aint base;

  /* compute displacements */
  disp[0] = MPI::Get_address(&p);
  disp[1] = MPI::Get_address(&p.m);
  disp[2] = MPI::Get_address(&p.x);
  disp[3] = MPI::Get_address(&p.v);

  base = disp[0];
  for (unsigned i=0; i<4; i++) disp[i] -= base;

  MPI_Particle = MPI::Datatype::Create_struct(4, blocklen, disp, type);
  MPI_Particle.Commit();

  if ( myrank == 0 )
  {
    p.id = 12;
    p.x[0] = p.x[1] = p.x[2] = 1.0;
    p.v[0] = p.v[1] = p.v[2] = 0.5;
    p.m = 42.0;

    std::cout << "The number of processes is " << numprocs << std::endl;

    // initialize data
    // send to every other process
    for (int i=1;i<numprocs;i++) {
      std::cout << " Sending to process " << i << std::endl;
      MPI::COMM_WORLD.Send(&p, 1, MPI_Particle, i, 1);
    }
  } else {
    // receive
    MPI::COMM_WORLD.Recv(&p, 1, MPI_Particle, 0, 1);
    std::cout << "  Process " << myrank << " got a particle with id " << p.id << std::endl;
  }

  // Terminate MPI.
  MPI::Finalize ( );
  // Process 0 says goodbye.
  if ( myrank == 0 )
  {
    wtime = MPI::Wtime ( ) - wtime;
    std::cout << "Elapsed wall clock time = " << wtime << " seconds." << std::endl;
  }
  return 0;
}

// vim:set et sts=2 ts=2 sw=2 ai ci cin:
