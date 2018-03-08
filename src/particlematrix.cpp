#include <cstdio>
#include <cmath>
#include "particlematrix.h"

ParticleMatrix::ParticleMatrix(int n, int s) {
    nof_particles = n;
    nof_slices = NOF_SLICES;
    size = s;

    particles = new particle_t[nof_particles];
    init_particles(nof_particles, particles);
}

void ParticleMatrix::index_particles(particle_matrix_t & particle_matrix) {

    particle_matrix_t slices(NOF_SLICES);


    for (int i = 0; i < nof_particles; i++) {
        int curr_particle_slice = (int) (particles[i].x*ceil(NOF_SLICES/size));

        if (curr_particle_slice >= nof_slices) {
            curr_particle_slice = nof_slices-1;
        } else if (curr_particle_slice < 0) {
            curr_particle_slice = 0;
        }

        slices[curr_particle_slice].push_back(&particles[i]);
    }

    for (int i = 0; i < nof_slices; i++) {
        particle_matrix.push_back(slices[i]);
    }
}
void ParticleMatrix::collision_check(particle_matrix_t & particle_matrix) {

#pragma omp parallel for
    for (int slice = 0; slice < NOF_SLICES; slice++) {
        int start = ((slice > 0) ? (slice - 1) : slice);
        int end = ((slice < nof_slices - 1) ? (slice + 2) : (slice + 1));

        for (int i = 0; i < particle_matrix[slice].size(); i++) {
            particle_t * curr_particle = particle_matrix[slice][i];
            (*curr_particle).ax = (*curr_particle).ay = 0;

            for (int j = start; j < end; j++) {
                for (int k = 0; k < particle_matrix[j].size(); k++) {
                    apply_force((*curr_particle), (*particle_matrix[j][k]));
                }
            }
        }
    }
}

void ParticleMatrix::perform_step() {
    std::vector <std::vector<particle_t *>> particle_matrix;

    index_particles(particle_matrix);

    collision_check(particle_matrix);

    for (int i = 0; i < nof_particles; i++) {
        move(particles[i]);
    }

}

particle_t * ParticleMatrix::get_particles() {
    return particles;
}