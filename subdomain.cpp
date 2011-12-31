#include "subdomain.hpp"

// standard constructor
SubDomain::SubDomain() : myrank(0), numprocs(1)
{
  for (unsigned dim = 0; dim < DIM; dim++)
    N_p[dim] = 0; 
}; 


// constructor for given myrank and given numprocs
SubDomain::SubDomain(int rank, int numprocessors) : myrank(rank), numprocs(numprocessors)
{
  for (unsigned dim = 0; dim < DIM; dim++)
    N_p[dim] = 0; 
}; 

