#include "OpenMP.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace OpenMP
{

void SetNumberOfThreads( int nthreads )
{
#ifdef _OPENMP
    omp_set_num_threads( nthreads );
#else
    kvs::IgnoreUnusedVariable( nthreads );
#endif
}

int GetNumberOfThreads()
{
#ifdef _OPENMP
    return omp_get_num_threads();
#else
    return 0;
#endif
}

int GetMaxThreads()
{
#ifdef _OPENMP
    return omp_get_max_threads();
#else
    return 0;
#endif
}

int GetThreadNumber()
{
#ifdef _OPENMP
    return omp_get_thread_num();
#else
    return 0;
#endif
}

int GetThreadLimit()
{
#ifdef _OPENMP
    return omp_get_thread_limit();
#else
    return 0;
#endif
}

int GetNumberOfProcessors()
{
#ifdef _OPENMP
    return omp_get_num_procs();
#else
    return 0;
#endif
}

bool InParallel()
{
#ifdef _OPENMP
    return static_cast<bool>( omp_in_parallel() );
#else
    return false;
#endif
}

void SetDynamic( bool dthreads )
{
#ifdef _OPENMP
    omp_set_dynamic( dthreads );
#else
    kvs::IgnoreUnusedVariable( dthreads );
#endif
}

bool GetDynamic()
{
#ifdef _OPENMP
    return static_cast<bool>( omp_get_dynamic() );
#else
    return 0;
#endif
}

void SetNested( bool nested )
{
#ifdef _OPENMP
    omp_set_nested( nested );
#else
    kvs::IgnoreUnusedVariable( nested );
#endif
}

bool GetNested()
{
#ifdef _OPENMP
    return static_cast<bool>( omp_get_nested() );
#else
    return 0;
#endif
}

void SetMaxActiveLevels( int max_levels )
{
#ifdef _OPENMP
    omp_set_max_active_levels( max_levels );
#else
    kvs::IgnoreUnusedVariable( max_levels );
#endif
}

int GetMaxActiveLevels()
{
#ifdef _OPENMP
    return omp_get_max_active_levels();
#else
    return 0;
#endif
}

int GetLevel()
{
#ifdef _OPENMP
    return omp_get_level();
#else
    return 0;
#endif
}

int GetAncestorThreadNumber( int level )
{
#ifdef _OPENMP
    return omp_get_ancestor_thread_num( level );
#else
    kvs::IgnoreUnusedVariable( level );
    return 0;
#endif
}

int GetTeamSize( int level )
{
#ifdef _OPENMP
    return omp_get_team_size( level );
#else
    kvs::IgnoreUnusedVariable( level );
    return 0;
#endif
}

int GetActiveLevel()
{
#ifdef _OPENMP
    return omp_get_active_level();
#else
    return 0;
#endif
}

double GetWTime()
{
#ifdef _OPENMP
    return omp_get_wtime();
#else
    return 0;
#endif
}

double GetWTick()
{
#ifdef _OPENMP
    return omp_get_wtick();
#else
    return 0;
#endif
}

Mutex::Mutex()
{
#ifdef _OPENMP
    omp_init_lock( &m_handler );
#endif
}

Mutex::~Mutex()
{
#ifdef _OPENMP
    omp_destroy_lock( &m_handler );
#endif
}

void Mutex::lock()
{
#ifdef _OPENMP
    omp_set_lock( &m_handler );
#endif
}

void Mutex::unlock()
{
#ifdef _OPENMP
    omp_unset_lock( &m_handler );
#endif
}

bool Mutex::tryLock()
{
#ifdef _OPENMP
    return omp_test_lock( &m_handler );
#else
    return false;
#endif
}

} // end of namespace OpenMP

} // end of namespace kvs
