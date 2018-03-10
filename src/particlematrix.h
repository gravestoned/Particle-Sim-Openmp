#ifndef PARTICLEMATRIX_H
#define PARTICLEMATRIX_H

#include "particles.h"
#include <vector>


class ParticleMatrix {
public:
    ParticleMatrix (int);               /* constructor, accepts number of particles and simulation size */
    void perform_step();                /* performs a step of simulation */
    particle_t * get_particles();       /* returns particle vector */
    void print();
private:
    typedef std::vector <particle_t *>  particle_vector_t;

    particle_t * particles;             /* main particle array */
    particle_vector_t * particle_matrix;
    int nof_slices;                     /* number of slices */
    int nof_particles;                  /* number of particles */
    double size;
};

#endif //PARTICLEMATRIX_H

