#ifndef __CS267_COMMON_H__
#define __CS267_COMMON_H__

#include "particles.h"
#include <iostream>

//
//  parameters
//

const int NSTEPS = 1000;
const int SAVEFREQ = 10;
const int DEF_SPEED = 1;
const int DEF_SIZE = 5;

//
//  timing routines
//
double read_timer( );

//
//  I/O routines
//

void save( FILE *f, int n, particle_t *p, double size );
std::string working_dir();

//
//  argument processing routines
//
int find_option( int argc, char **argv, const char *option );
int read_int( int argc, char **argv, const char *option, int default_value );
char *read_string( int argc, char **argv, const char *option, char *default_value );

//
// Visualizer
//

void run_visualizer(char *);

#endif
