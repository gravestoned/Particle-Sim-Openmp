#ifndef PARTICLES_H
#define PARTICLES_H

//
// particle data structure
//

typedef struct
{
    double x;
    double y;
    double vx;
    double vy;
    double ax;
    double ay;
} particle_t;

void set_size( int n );
void init_particles( int n, particle_t *p );
void apply_force( particle_t &particle, particle_t &neighbor );
void move( particle_t &p );
void copy_particle( particle_t *src, particle_t *dst);

#endif
