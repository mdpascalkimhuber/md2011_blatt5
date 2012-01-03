/** \mainpage MD Praktikum 2011
 *
 * \section blatt5_sec Übungsblatt 5
 *
 * Moleküldynamiksimulation
 *
 * \subsection algorithms_subsec Implementierte Verfahren und deren Klassen
 *
 * - VelocityVerlet_LC mit \f$O(N^2)\f$ Kraftberechnungen
 * - Linked_Cell Methode
 * - MPI
 *
 * \subsection classes_subsec Weitere modifizierte Klassen
 *
 * - World
 * - World_LC
 * - Observer
 * - ObserverXYZ
 * - ObserverXYZ_LC
 * - SubDomain
 *
 * \subsection manual_sec Anleitung
 *
 * \code
 * make && ./blatt5
 * \endcode
 *
 */
#ifndef _DEFINES_HPP
#define _DEFINES_HPP

// define the dimension of the particles
#define DIM 3
// reals in double precision
typedef double real;
// square define
#define sqr(_x) ((_x)*(_x)) 
// define BorderType
enum BorderType {unknown = 0, leaving = 1, periodic = 2};

#endif // _DEFINES_HPP
// vim:set et sts=4 ts=4 sw=4 ai ci cin cino=g0,t0:
