#include "common.h"
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <cmath>
#include "windows.h"

//
//  timer
//
double read_timer( )
{
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

//
//  I/O routines
//
void save( FILE *f, int n, particle_t *p, double size)
{
    static bool first = true;
    double size_coef = size / sqrt( 0.0005 * n );
    if( first )
    {
        fprintf( f, "%d %g\n", n, size );
        first = false;
    }
    for( int i = 0; i < n; i++ )
        fprintf( f, "%g %g\n", p[i].x * size_coef, p[i].y * size_coef);
}

std::string working_dir() {
    char buf[256];
    GetCurrentDirectoryA(256, buf);
    return std::string(buf) + "\\";
}

//
//  command line option processing
//
int find_option( int argc, char **argv, const char *option )
{
    for( int i = 1; i < argc; i++ )
        if( strcmp( argv[i], option ) == 0 )
            return i;
    return -1;
}

int read_int( int argc, char **argv, const char *option, int default_value )
{
    int iplace = find_option( argc, argv, option );
    if( iplace >= 0 && iplace < argc-1 )
        return atoi( argv[iplace+1] );
    return default_value;
}

char *read_string( int argc, char **argv, const char *option, char *default_value )
{
    int iplace = find_option( argc, argv, option );
    if( iplace >= 0 && iplace < argc-1 )
        return argv[iplace+1];
    return default_value;
}

//
// Start visualizer
//

void run_visualizer(char * filename) {
    /* build path to program and savefile */
    std::string path = working_dir();
    std::string arg = path + filename;
    path.replace(path.length()-18, 18, "util\\visualize.exe");

    ShellExecute(NULL, "open", path.c_str(), arg.c_str(), NULL, SW_SHOWDEFAULT);
}