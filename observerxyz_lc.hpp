#ifndef _OBSERVERXYZ_LC_HPP
#define _OBSERVERXYZ_LC_HPP

#include "observer.hpp"
#include "world_lc.hpp"

/**
 * @brief Subclass from Observer: an observer for the
 * timediscretization with xyz_output and linked_cell structure
 */
class ObserverXYZ_LC : public Observer
{
public: 
  /* @brief constructor
   *
   * opens and creates the files written during observation 
   *
   * @brief _W
   */
  ObserverXYZ_LC(World_LC &_W);  
  
  /* @brief destructor
   *
   * closes all files especially the xyz_file
   *
   */
  ~ObserverXYZ_LC();

  /**
   * @brief notify the observer that the world has changed
   */
  virtual void notify();

  /**
   * @brief output the coordinates of all particles of one specific
   * cell C in xyz_format
   * 
   * @param Cell &C
   *
   */
  virtual void output_xyz_in(Cell &C); 
 
  /**
   * @brief output coordinates of the particles in xyz_file
   */
  virtual void output_xyz(); 

protected: 
  // World_LC
  World_LC &W_LC; 
  // xyz_filestream
  std::ofstream coordinates_xyz; 

}; 

#endif //_OBSERVERXYZ_LC_HPP
