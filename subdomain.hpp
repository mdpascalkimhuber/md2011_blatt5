#ifndef _SUBDOMAIN_HPP
#define _SUBDOMAIN_HPP

#include "defines.hpp"

/** 
 * @brief subdomain class for parallelization 
 */
class SubDomain {
public: 
  // data about world
  /// world size 
  real L[DIM]; 
  /// number of cells in world
  int N_c[DIM]; 

  // data for parallelization
  /// process id
  int myrank; 
  /// number of started processes
  int numprocs; 
  /// position of process in process-grid 
  int ip[DIM]; 
  /// size of process-grid in every dimension 
  int N_p[DIM]; 
  /// process ids of neighbouring processes (lower borders)
  int ip_lower[DIM]; 
  /// process ids of neighbouring processes (upper borders)
  int ip_upper[DIM]; 
  /// width of margin (normaly 1 in every dimension)
  int ic_start[DIM]; 
  /// upper corner of margin
  int ic_stop[DIM]; 
  /// number of cells in subdomain (with margin)
  int ic_number[DIM]; 
  /// size of a cell
  real cellh[DIM]; 
  /// lower corner of subdomain (with margin)
  int ic_lower_global[DIM]; 
}; 
  
#endif // _SUBDOMAIN_HPP
