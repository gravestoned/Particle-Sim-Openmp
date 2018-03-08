#include <random>
#include "particles.h"

double size = 5;

//
//  tuned constants
//
#define density 0.0005
#define mass    0.01
#define cutoff  0.05
#define min_r   (cutoff/100)
#define dt      0.0005


//
// copies particle from src to dst
//
void copy_particle(particle_t * src, particle_t * dst) {

    dst->x = src->x;
    dst->y = src->y;

    dst->ax = src->ax;
    dst->ay = src->ay;

    dst->vx = src->vx;
    dst->vy = src->vy;
}

void print_particle(particle_t particle) {
    printf("x: %f, y: %f, ax: %f, ay: %f, vx: %f, vy: %f\n", particle.x,  particle.y,  particle.ax,  particle.ay,  particle.vx,  particle.vy);
}

//
//  keep density constant
//
void set_size( int n )
{
    size = n; //sqrt( density * n );
}

//
//  Initialize the particle positions and velocities
//
void init_particles( int n, particle_t *p )
{

    /* random init code */
    std::random_device device;
    std::mt19937 rnd(device());
    std::uniform_real_distribution<double> dist(1.0, 100.0);

    int sx = (int)ceil(sqrt((double)n));
    int sy = (n+sx-1)/sx;

    int *shuffle = (int*)malloc( n * sizeof(int) );
    for( int i = 0; i < n; i++ )
        shuffle[i] = i;

    for( int i = 0; i < n; i++ )
    {
        //
        //  make sure particles are not spatially sorted
        //
        int j = (int) dist(rnd)%(n-i);
        int k = shuffle[j];
        shuffle[j] = shuffle[n-i-1];

        //
        //  distribute particles evenly to ensure proper spacing
        //
        p[i].x = size*(1.+(k%sx))/(1+sx);
        p[i].y = size*(1.+(k/sx))/(1+sy);

        //
        //  assign random velocities within a bound
        //
        p[i].vx = dist(rnd)*2-1;
        p[i].vy = dist(rnd)*2-1;
    }
    free( shuffle );
}

//
//  interact two particles
//
void apply_force( particle_t &particle, particle_t &neighbor )
{

    double dx = neighbor.x - particle.x;
    double dy = neighbor.y - particle.y;
    double r2 = dx * dx + dy * dy;
    if( r2 > cutoff*cutoff )
        return;
    r2 = fmax( r2, min_r*min_r );
    double r = sqrt( r2 );

    //
    //  very simple short-range repulsive force
    //
    double coef = ( 1 - cutoff / r ) / r2 / mass;
    particle.ax += coef * dx;
    particle.ay += coef * dy;

}

//
//  integrate the ODE
//
void move( particle_t &p )
{

    //  slightly simplified Velocity Verlet integration
    //  conserves energy better than explicit Euler method
    //
    p.vx += p.ax * dt;
    p.vy += p.ay * dt;
    p.x  += p.vx * dt;
    p.y  += p.vy * dt;

    //
    //  bounce from walls
    //
    while( p.x < 0 || p.x > size )
    {
        p.x  = p.x < 0 ? -p.x : 2*size-p.x;
        p.vx = -p.vx;
    }
    while( p.y < 0 || p.y > size )
    {
        p.y  = p.y < 0 ? -p.y : 2*size-p.y;
        p.vy = -p.vy;
    }
}

