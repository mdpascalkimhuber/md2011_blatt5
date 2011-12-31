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


// compute ip of subdomain with myrank and N_p
void SubDomain::comp_ip()
{
  // initialize helper variable
  int index = myrank; 
  // calculate ip with myrank (similar to compute_cell_pos)
  for (unsigned dim = DIM; dim > 0; dim--)
    {
      ip[dim-1] = index % N_p[dim-1]; 
      index = (index - ip[dim-1])/N_p[dim-1]; 
    }
}; 


// compute rank of a subdomain on the basis of the ip
int SubDomain::comp_otherrank(int other_ip[DIM])
{
  int otherrank = 0; 
  // calculate other_rank: similar to compute_global
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      otherrank *= N_p[dim]; 
      otherrank += other_ip[dim];
    }

  // return value
  return otherrank; 

}; 
