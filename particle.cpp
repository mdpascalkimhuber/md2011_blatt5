#include <iostream>
#include <mpi.h>
#include "defines.hpp"
#include "particle.hpp"


std::ostream& operator << (std::ostream &Stream, const Particle& P)
{
  // write mass of particle
  Stream << "m: " << P.m << std::endl;

  // write coordinates of particle
  Stream << "x: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.x[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl; 

  // write velocities of particle
  Stream << "v: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.v[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl;

  // write forces of particle
  Stream << "F: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.F[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl;

  // write old forces of particle
  Stream << "F_old: ( "; 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      Stream << P.F_old[dim]; 
      if (dim < DIM-1)
	{
	  Stream << ", "; 
	}
    }
  Stream << ")" << std::endl << std::endl;

  return Stream;
};
    
void build_particle(MPI::Datatype& MPI_Particle)
{
  // Initialize Particle
  Particle p;

  // build new mpi datatype
  MPI::Datatype type[6] = {MPI::DOUBLE, MPI::DOUBLE, MPI::DOUBLE, MPI::DOUBLE, MPI::DOUBLE, MPI::DOUBLE}; 
  int blocklen[6] = {1,1,DIM,DIM,DIM,DIM};
  MPI::Aint disp[6]; // displacements
  MPI::Aint base;

  /* compute displacements */
  disp[0] = MPI::Get_address(&p);
  disp[1] = MPI::Get_address(&p.m);
  disp[2] = MPI::Get_address(&p.x);
  disp[3] = MPI::Get_address(&p.v);
  disp[4] = MPI::Get_address(&p.F); 
  disp[5] = MPI::Get_address(&p.F_old); 

  base = disp[0];
  for (unsigned i=0; i<6; i++) disp[i] -= base;

  MPI_Particle = MPI::Datatype::Create_struct(6, blocklen, disp, type);
  MPI_Particle.Commit();
}
    

