#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <mpi.h>

#include "world_lc.hpp"
#include "gravitypotential.hpp"
#include "ljpotential.hpp"
#include "velocityverlet_lc.hpp"
#include "observerxyz_lc.hpp"


using namespace std;
int main(int argc, char *argv[]) {

  // check arguments
  if (argc < 2) {
    std::cerr << "error: missing arguments" << std::endl;
    std::cerr << "usage: " << std::endl
              << "\t" << argv[0] << " parameterfile particledatafile" << std::endl;
    return EXIT_FAILURE;
  }

  // instanciate variables for parallelization
  int myrank, numprocs; 

  // initialize MPI
  MPI::Init(argc, argv); 
  
  // determine number and id of process(es)
  numprocs = MPI::COMM_WORLD.Get_size(); 
  myrank = MPI::COMM_WORLD.Get_rank(); 

  /*--------------------------------------------------------------------------------
    Calculations for the SubDomain
    --------------------------------------------------------------------------------*/

  // instantiate Potential
  LJ_Potential Pot(2.5);

  // create World
  World_LC W(myrank, numprocs); 
  
  // read Parameters
  W.read_Parameter(argv[1]);

  // read Particles
  W.read_Particles(argv[2]);
    
  // print World configuration
  std::cout << W << std::endl; 

  // print Cell configuration 
  for ( unsigned index = 0; index < W.cells.size(); index++)
    {
      cout << W.cells[index]; 
    }

  // create the Observer
  ObserverXYZ_LC O(W);

  // instanciate timediscretization 
  VelocityVerlet_LC Verlet(W, Pot, O);

  // run the simulation
  Verlet.simulate();

  // terminate MPI
  MPI::Finalize(); 
  
  // print Cell configuration 
  for ( unsigned index = 0; index < W.cells.size(); index++)
    {
      cout << W.cells[index]; 
    }

  return EXIT_SUCCESS;
}

// vim:set et sts=2 ts=2 sw=2 ai ci cin:
