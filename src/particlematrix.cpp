#include <cstdio>
#include <cmath>
#include "particlematrix.h"

ParticleMatrix::ParticleMatrix(int n) {
    nof_particles = n;
    nof_slices = n/10;

    size = sqrt(0.0005 * n);

    set_size(n);

    particle_matrix.resize(nof_slices);

    particles = new particle_t[nof_particles];
    init_particles(nof_particles, particles);
}

void ParticleMatrix::index_particles() {

    particle_matrix.clear();
    particle_matrix.resize(nof_slices);

    for (int i = 0; i < nof_particles; i++) {
        auto curr_particle_slice = (particles[i].x*ceil(nof_slices/size));

        if (curr_particle_slice >= nof_slices) {
            curr_particle_slice = nof_slices-1;
        } else if (curr_particle_slice < 0) {
            curr_particle_slice = 0;
        }

        particle_matrix[curr_particle_slice].push_back(&particles[i]);
    }

}
void ParticleMatrix::collision_check() {
#pragma omp parallel for
    for (int slice = 0; slice < nof_slices; slice++) {
        int start = ((slice > 0) ? (slice - 1) : slice);
        int end = ((slice < nof_slices - 1) ? (slice + 2) : (slice + 1));

        for (particle_t * curr_particle: particle_matrix[slice]) {
            (*curr_particle).ax = (*curr_particle).ay = 0;

            for (int j = start; j < end; j++) {
                for (particle_t * neigh_particle: particle_matrix[j]) {
                    apply_force((*curr_particle), (*neigh_particle));
                }
            }
        }
    }
}

void ParticleMatrix::perform_step() {
    index_particles();

    collision_check();

    for (int i = 0; i < nof_particles; i++) {
        move(particles[i]);
    }

}

particle_t * ParticleMatrix::get_particles() {
    return particles;
}