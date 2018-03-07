#include "particlematrix.h"

ParticleMatrix::ParticleMatrix(int n) {
    nof_particles = n;
    nof_slices = 10;

    particles = new particle_t[nof_particles];
    particle_storage = new particle_t[nof_particles];

    init_particles(nof_particles, particles);

    particle_storage = new particle_t[nof_particles];

    particle_index_vector = new int[nof_particles];
    slice_lengths = new int[nof_slices];
    slice_sizes = new int[nof_slices];
}

void ParticleMatrix::index_particles() {
    int curr_index;


#pragma omp parallel for
    for (int i = 0; i < nof_slices; i++) {
        slice_lengths[i] = 0;
        slice_sizes[i] = 0;
    }

    /* calculates and saves particle slice index and size */
#pragma omp for
    for (int i = 0; i < nof_particles; i++) {
        curr_index = (int) (particles[i].x*2)-1;

        if (curr_index >= nof_slices) {
            curr_index = nof_slices-1;
        } else if (curr_index < 0) {
            curr_index = 0;
        }

        particle_index_vector[i] = curr_index;
        slice_lengths[curr_index]++;
    }

    /* builds new particle matrix */
#pragma omp for
    for (int i = 0; i < nof_particles; i++) {
        curr_index = 0;

        for (int j = 0; j < particle_index_vector[i]; j++) {
            curr_index += slice_lengths[j];
        }

        curr_index += slice_sizes[particle_index_vector[i]];

        copy_particle(&particles[i], &particle_storage[curr_index]);
        slice_sizes[particle_index_vector[i]]++;
    }

    /* transfers newly built matrix to main particle matrix */
#pragma omp for
    for (int i = 0; i < nof_particles; i++) {
        copy_particle(&particle_storage[i], &particles[i]);
    }
}
void ParticleMatrix::collision_check(int slice) {
    int start = ((slice > 0) ? (slice - 1) : slice);
    int end = ((slice < nof_slices - 1) ? (slice + 2) : (slice + 1));

#pragma omp parallel for
    for (int i = 0; i < slice_lengths[slice]; i++) {
        particles[get_index(slice,i)].ax = particles[get_index(slice,i)].ay = 0;

        for (int j = start; j < end; j++) {
            for (int k = 0; k < slice_lengths[j]; k++) {
                apply_force(particles[get_index(slice, i)], particles[get_index(j, k)]);
            }
        }
    }
}
void ParticleMatrix::perform_step() {
    index_particles();

#pragma omp parallel for
    for (int i = 0; i < nof_slices; i++) {
        collision_check(i);
    }

#pragma omp parallel for
    for (int i = 0; i < nof_particles; i++) {
        move(particles[i]);
    }

}

int ParticleMatrix::get_index(int slice, int pos) {
    int index = 0, i;

    for (i = 0; i < slice; i++) {
        index += slice_lengths[i];
    }

    index += pos;

    return index;
}

particle_t * ParticleMatrix::get_particles() {
    return particles;
}