#include "world_lc.hpp"
#include <math.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

// standard constructor
World_LC::World_LC() : World(), cell_r_cut(0), particles_N(0), global_cell_N(1), set_start_temperature(0), random_seed(0), thermostat_step_interval(0), thermostat_target_temperature(0)
{
  // empty constructor
}; 

// constructor for process myrank
World_LC::World_LC(int rank, int numprocessors) : World(), cell_r_cut(0), particles_N(0), global_cell_N(1), set_start_temperature(0), random_seed(0), thermostat_step_interval(0), thermostat_target_temperature(0), s(rank, numprocessors)
{
  // empty constructor
}

// calculate index of a cell with cell_coordinates
unsigned World_LC::compute_global(const int (&cell_pos)[DIM])
{
  // helper
  unsigned index = 0; 

  // calculate cell_index by formula $ j = j_3 + N_3(j_2 + N_2j_1) $
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      index *= cell_N[dim]; 
      index += cell_pos[dim];
    }

  // return value
  return index; 
}

// calculate the index of a cell with given coordinates pos[DIM]
unsigned World_LC::compute_cell_index(const real (&pos)[DIM]) 
{
  // helper variables
  unsigned index = 0;
  
  // calculate cell_index by formula $ j = j_3 + N_3(j_2 + N_2j_1) $ 
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      index *= cell_N[dim]; 
      index += int(pos[dim]/cell_length[dim]); 
    }

  // return value
  return index; 
}; 


// calculate the position of a cell (index_vector) with the global index
void World_LC::compute_cell_pos(unsigned index, int (&cell_pos)[DIM])
{
  // calculate global cell index and save it in cell_pos (cell_pos
  // is the index_vector of a cell)
  for (unsigned dim = DIM; dim > 0; dim--)
    {
      cell_pos[dim-1] = index % cell_N[dim-1]; 
      index = (index - cell_pos[dim-1])/cell_N[dim-1]; 
    }
}; 


// derived read_Parameter: call original read_Parameter and open file
// again to read additional parameters
void World_LC::read_Parameter(const std::string &filename) 
{
  /*--------------------------------------------------------------------------------
    read *.parameter-file 
    --------------------------------------------------------------------------------*/

  // call original member function to read basic parameters
  World::read_Parameter(filename); 
  
  // create input filestream to open file again
  std::ifstream parfile(filename.c_str()); 
  // check if file is open
  if (!parfile.is_open()) 
    throw std::runtime_error(
			     "read_Parameter(): Can't open file '" + filename + "' for reading."
			     ); 
  // helper strings
  std::string line, option; 

  // read file till eof
    while ( parfile.good())
    {
      // read line from file
      getline(parfile, line); 
      // create a string stream
      std::stringstream strstr; 
      // put line into string stream 
      strstr << line; 
      // read option and value from stringstream
      strstr >> option; 
      // check option and read values
      if (option=="cell_r_cut")
	strstr >> cell_r_cut; 
      if (option=="set_start_temperature")
	strstr >> set_start_temperature;
      if (option == "thermostat_step_interval")
	strstr >> thermostat_step_interval;
      if (option == "thermostat_target_temperature")
	strstr >> thermostat_target_temperature;
      if (option == "random_seed")
	strstr >> random_seed;
      if (option == "procs")
	{
	  // for each dimension
	  for (unsigned dim = 0; dim < DIM; dim++)
	    strstr >> s.N_p[dim]; 
	}
    }

  // close file
  parfile.close(); 


  /*--------------------------------------------------------------------------------
    calculate all cell specific data
    --------------------------------------------------------------------------------*/

  // calculating number of cells and length of cells
  for (unsigned dim = 0; dim < DIM; dim++) 
    {
      // calculate number of cells: the typecast allows a cell
      // length superior to cell_r_cut
      cell_N[dim] = std::max(1, int(world_size[dim]/cell_r_cut)); 
      
      // calculate length of cell (implicit typecast of cell_N)
      cell_length[dim] = world_size[dim]/cell_N[dim]; 

      // calculating total number of cells
      global_cell_N *= cell_N[dim]; 
    }
  


  /*--------------------------------------------------------------------------------
    instanciate all variables of the subdomain
    --------------------------------------------------------------------------------*/
  
  // calculate ip of subdomain
  s.comp_ip(); 
  // set length of whole world, cell_length, number of cells in whole
  // world and other variables
  for (unsigned dim = 0; dim < DIM; dim++)
    {
      // Length of whole world
      s.L[dim] = world_size[dim]; 
      // total of cells in whole world
      s.N_c[dim] = cell_N[dim]; 
      // length of cells
      s.cellh[dim] = cell_length[dim]; 
      // set borderwidth of subdomain to 1 as default
      s.ic_start[dim] = int(cell_length[dim]/cell_r_cut); 
      // calculate ic_stop (no. of cells in subdomain) (the borders of
      // subdomain are supposed to coincident wiht those of the cells)  
      s.ic_stop[dim] = int(cell_N[dim] / s.N_p[dim]) + s.ic_start[dim]; 
      // calculate number of cells in subdomain with border_cells
      s.ic_number[dim] = s.ic_stop[dim] + s.ic_start[dim]; 
      // calculate global index of first cell in (inner) subdomain
      s.ic_lower_global[dim] = s.ip[dim]*(s.ic_stop[dim] - s.ic_start[dim]); 
    }


  /*--------------------------------------------------------------------------------
    create cells for subdomain
    --------------------------------------------------------------------------------*/
  
  // helper_variable
  int total_cell_number = 1; 
  // just a helper cell
  Cell new_cell; 

  // initialize total_cell_number
  for (unsigned dim = 0; dim < DIM; dim++)
    total_cell_number *= s.ic_number[dim]; 

  // Adding cells to world and writing some information in them
  for (int c_idx = 0; c_idx < total_cell_number; c_idx++)
    {
      // give an id to cell
      new_cell.id = c_idx + 1; 

      // give cell_index_pos to cell
      compute_cell_pos(c_idx, new_cell.cell_pos); 
      
      // add cell to cell_vector in world
      cells.push_back(new_cell); 
    }

};


// derived read_Particles: call the original member function and
// distribute all particles over the cells 
void World_LC::read_Particles(const std::string &filename)
{
  // call original read_Particles of the basis class World
  World::read_Particles(filename); 

  // instanciate total number of particles in particles_N
  particles_N = 0; 

  // helper variable
  unsigned index; 
  // corrected position of particle for subdomain
  real sub_pos[DIM]; 
  // bool to check if particle is in right subdomain
  bool in_subdomain; 
  // helper iterator for particle-vector
  std::list<Particle>::iterator itparticle = particles.begin(); 
  
  // delete particles of particles-vector and put them in right cells
  while (itparticle != particles.end())
    {
      // check if itparticle is in right domain
      // instanciate bool-variable
      in_subdomain = true; 
      for (unsigned dim = 0; dim < DIM; dim++)
	{ // lower border
	  if (itparticle->x[dim] < (real(s.ic_lower_global[dim])*s.cellh[dim]))
	    in_subdomain = false; 
	  // upper border
	  else if (itparticle->x[dim] >= (real(s.ic_lower_global[dim]+(s.ic_stop[dim]-s.ic_start[dim]))*s.cellh[dim]))
	    in_subdomain = false; 
	}

      // if particle is not in right domain, delete it
      if (in_subdomain = false)
	{
	  itparticle = particles.erase(itparticle); 
	}
      // if it is in right domain, put it in the right (inner) cell
      else
	{
      	  // instanciate sub_pos (corrected positions for subdomain)
	  for (unsigned dim = 0; dim < DIM; dim++) 
	    { // corrected position: attention to border cells
	      sub_pos[dim] = itparticle->x[dim] - (real(s.ic_lower_global[dim] - s.ic_start[dim])*s.cellh[dim]); 
	    }
	  
	  // calculate the index of the right cell 
	  index = compute_cell_index(sub_pos); 

	  // add particle to particles-vector in the right cell
	  cells[index].particles.push_back(particles.front()); 

	  // update number of particles in subdomain
	  particles_N++;  

	  // delete particle in world_particles-vector
	  itparticle = particles.erase(itparticle); 
	}
    }
  // Calculating new velocities for each particle if start_temperature
  /*--------------------------------------------------------------
    ONLY FOR 2D: Maxwell-Boltzmann-distribution
    ---------------------------------------------------------------*/
  if (thermostat_target_temperature != 0)
    {
       // initialize random number generator
      if ( random_seed < 1)
	{
	  srand( time(NULL) );
	}
      else 
	{
	  srand ( random_seed );
	}

      //initialize iterator for cell vector
      std::vector<Cell>::iterator cell = cells.begin();
      
      // go through every cell
      while ( cell != cells.end())
	{
	  //initialize iterator for particles
	  std::list<Particle>::iterator p_cell = cell->particles.begin();
	  
	  // calculate all velocities
	  while (p_cell != cell->particles.end())
	    {
	      // calculate velocities by Maxwell_Boltzmann
	      
	      // helper variables
	      real u_1;
	      real u_2;
	      real s = 2.0; 
	      real r = 1.0; 

	      do {
		// generate two doubles in the interval [0.0 , 1.0]
		u_1 = (double(rand() ) / RAND_MAX); 
		u_2 = (double(rand() ) / RAND_MAX);
		// generate two doubles in the interval [-1.0 , 1.0]
		u_1 = 2.0 * u_1 -1.0;
		u_2 = 2.0 *u_2 -1.0;
		// normalize
		s = u_1 *u_1 + u_2 *u_2;
	      } while (s > 1.0);

	      r = -2.0 *log(s) /s;
	      // set random velocities for particle
	      p_cell->v[0] = u_1 *sqrt(r);
	      p_cell->v[1] = u_2 *sqrt(r);
	      p_cell->v[2] = 0.0; 

	      // increment iterator for while-loop over particles
	      p_cell++;
	    }
	  // increment iterator for while-loop over cells
	  cell++;
	}
    }
  
}


std::ostream& operator << (std::ostream& os, World_LC& W) {
  os << "t=" << W.t << " delta_t=" << W.delta_t << " t_end=" << W.t_end
     << " Number of Particles=" << W.particles_N 
     << " cell_r_cut=" << W.cell_r_cut
     << " Number of cells=" << W.cells.size() << std::endl; 
  
  // give out number of cells in every dimension
  for (unsigned dim = 0; dim < DIM; dim++)
    os << "Cell_N[" << dim << "]=" << W.cell_N[dim] << " "; 
  return os << std::endl; 
};
