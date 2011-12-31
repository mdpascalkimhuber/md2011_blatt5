#include "subdomain.hpp"

// standard constructor
SubDomain::SubDomain() : myrank(0), numprocs(1)
{
  // empty constructor
}; 


// constructor for given myrank and given numprocs
SubDomain::SubDomain(int rank, int numprocessors) : myrank(rank), numprocs(numprocessors)
{
  // empty constructor
}; 

