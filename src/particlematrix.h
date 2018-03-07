#ifndef PARTICLEMATRIX_H
#define PARTICLEMATRIX_H

#include "particles.h"

class ParticleMatrix {
    particle_t * particles;             /* main particle matrix */
    particle_t * particle_storage;      /* used to store particles during indexation */
    int nof_slices;                     /* number of slices */
    int * slice_lengths;                /* length of corresponding slice vectors */
    int * slice_sizes;                  /* size of slice vector, used for indexation */
    int * particle_index_vector;        /* particle index storage vector, used for indexation */
    int nof_particles;                  /* number of particles */
public:
    ParticleMatrix (int);               /* constructor, accepts number of particles */
    void perform_step();                /* performs a step of simulation */
    particle_t * get_particles();       /* returns particle vector */
private:
    void index_particles();             /* performs particle indexation */
    void collision_check(int);          /* performs a collision check on given slice */
    int get_index(int,int);             /* return the location of particle in main matrix with given slice and position */
};

#endif //PARTICLEMATRIX_H

