#include "subdomain.hpp"
#include <iostream>

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
int SubDomain::comp_otherrank(const int other_ip[DIM])
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


// calculate ranks of neighbouring subdomains
void SubDomain::comp_neighbouring_ip(const BorderType borders[DIM][2])
{
  // declare helper variables 
  int other_ip[DIM]; 
  bool no_ip;

  // instanciate other_ip and loop_index
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      other_ip[dim] = ip[dim]; 
    }
  
  // go over the neighbour subdomains
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      // lower ip
      other_ip[dim] = ip[dim] - 1; 
      no_ip = false; 

      if (other_ip[dim] < 0)
	{ // check border type
	  switch (borders[dim][0])
	    {
	    case unknown: 
	      { // for unknown bordertype send a message
		std::cout << "BorderType is unknown. ip of neighbouring subdomain can't be calculated." << std::endl; 
		no_ip = true; 
		break; 
	      }
	    case leaving: 
	      { // set neighbouring rank to -1 for later handling
		no_ip = true; 
		break; 
	      }
	    case periodic: 
	      { // choose right subdomain in periodic case
		other_ip[dim] = N_p[dim]-1; 
		break; 
	      }
	    }
	}
      // calculate new rank for ip_lower
      if (no_ip = true)
	ip_lower[dim] = -1; 
      else
	ip_lower[dim] = comp_otherrank(other_ip); 

      // instanciate other_ip for ip_upper
      other_ip[dim] = ip[dim]; 

      // upper ip
      other_ip[dim] = ip[dim] + 1; 
      no_ip = false; 
      if (other_ip[dim] >= N_p[dim])
	{ // check border type 
	  switch (borders[dim][1])
	    {
	    case unknown: 
	      { // for unknown bordertype send a message
		std::cout << "BorderType is unknown. ip of neighbouring subdomain can't be calculated." << std::endl; 
		no_ip = true; 
		break; 
	      }
	    case leaving: 
	      { // set neighbouring rank to -1 for later handling
		no_ip = true; 
		break; 
	      }
	    case periodic: 
	      { // choose right subdomain in periodic case
		other_ip[dim] = 0; 
		break; 
	      }
	    }
	}
      // calculate new rank for ip_upper
      if (no_ip = true)
	ip_upper[dim] = -1; 
      else
	ip_upper[dim] = comp_otherrank(other_ip); 

      // instanciate other_ip for next loop 
      other_ip[dim] = ip[dim]; 
    }
}; 

