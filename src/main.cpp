#include <unistd.h>
#include "common.h"
#include "particlematrix.h"

//
//  benchmarking program
//


int main( int argc, char **argv )
{
    if( find_option( argc, argv, "-h" ) >= 0 )
    {
        printf( "Options:\n" );
        printf( "-h to see this help\n" );
        printf( "-n <int> to set the number of particles (default: 1000)\n" );
        printf( "-o <filename> to specify the output file name\n" );
        printf( "-s <int> to specify the size of the simulation area (default: 5)\n" );
        printf( "-t <int> slows the simulation down by given factor (default: 1)\n" );
        printf( "-v to run visualiser after simulation (default: off)\n" );
        printf( "-sf <int> to specify the save frequency (default: 10)\n" );
        return 0;
    }

    int n = read_int( argc, argv, "-n", 1000 );
    int slowdown = read_int( argc, argv, "-t", DEF_SPEED);
    int size = read_int( argc, argv, "-s", DEF_SIZE);

    bool visualize = (0 < find_option( argc, argv, "-v"));
    char *savename = read_string( argc, argv, "-o", NULL);
    int save_frequency = read_int( argc, argv, "-sf", SAVEFREQ);
    FILE *fsave = savename ? fopen( savename, "w" ) : NULL;

    ParticleMatrix matrix (n);


    //
    //  simulate a number of time steps
    //


    double simulation_time = read_timer( );
    double save_time_start, save_time_total = 0;

    for( int step = 0; step < NSTEPS; step++ )
    {

        if (step%slowdown==0) {
            matrix.perform_step();
        }

        //
        //  save if necessary
        //
        if( fsave && (step%save_frequency) == 0) {
            save_time_start = read_timer();
            save(fsave, n, matrix.get_particles(), size);
            save_time_total += read_timer()-save_time_start;
        }
    }
    simulation_time = read_timer( ) - simulation_time - save_time_total;

    printf( "n = %d, simulation time = %g seconds\n", n, simulation_time );

    matrix.print();
    if( fsave ) {
        fclose(fsave);

        if (visualize) {
            run_visualizer(savename);
        }
    }
    return 0;
}

