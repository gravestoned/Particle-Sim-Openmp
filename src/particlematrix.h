#ifndef PARTICLEMATRIX_H
#define PARTICLEMATRIX_H

#include "particles.h"
#include <vector>

#define NOF_SLICES 10

class ParticleMatrix {
    particle_t * particles;             /* main particle matrix */
    int nof_slices;                     /* number of slices */
    int nof_particles;                  /* number of particles */
    int size;
public:
    ParticleMatrix (int,int);               /* constructor, accepts number of particles and simulation size */
    void perform_step();                /* performs a step of simulation */
    particle_t * get_particles();       /* returns particle vector */
private:
    typedef std::vector <particle_t *> particle_vector_t;
    typedef std::vector <particle_vector_t> particle_matrix_t;
    void index_particles(particle_matrix_t &);             /* performs particle indexation */
    void collision_check(particle_matrix_t &);                                                                 /* performs a collision check on given slice */
};

#endif //PARTICLEMATRIX_H

