#ifndef _PARTICLE_HPP
#define _PARTICLE_HPP

#include "defines.hpp"
#include <iostream>
#include <mpi.h>

/**
 * @brief Particle data
 *
 * this class contains the particle data
 */
class Particle {
public:
  /// id
  real id; 
  /// Mass
  real m;
  /// Position
  real x[DIM];
  /// Velocity
  real v[DIM];
  /// Force
  real F[DIM];
  /// Force (previous step)
  real F_old[DIM];
};

/* 
 * @brief build MPI_Datatype MPI_Particle
 */
void build_particle(MPI::Datatype& MPI_Particle); 

/*
 * @brief output operator for particles
 */
  std::ostream& operator << (std::ostream &Stream, const Particle &P);

#endif // _PARTICLE_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin:
