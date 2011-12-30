#ifndef _SUBDOMAIN_HPP
#define _SUBDOMAIN_HPP

#include "defines.hpp"

/** 
 * @brief subdomain class for parallelization 
 */
class SubDomain {
public: 
  /// data about world
  // world size 
  real L[DIM]; 
  // number of cells in world
  int N_c[DIM]; 

  /// data for parallelization
  // process id
  int myrank; 
  // number of started processes
  int numprocs; 
  // position of process in process-grid 
  int ip[DIM]; 
  // size of process-grid in every dimension 
  
