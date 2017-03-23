#pragma once
#include "OMP.h"


namespace kvs
{

namespace OpenMP
{

void SetNumberOfThreads( int nthreads );
int GetNumberOfThreads();
int GetMaxThreads();
int GetThreadNumber();
int GetThreadLimit();
int GetNumberOfProcessors();
bool InParallel();
void SetDynamic( bool dthreads );
bool GetDynamic();
void SetNested( bool nested );
bool GetNested();
void SetMaxActiveLevels( int max_levels );
int GetMaxActiveLevels();
int GetLevel();
int GetAncestorThreadNumber( int level );
int GetTeamSize( int level );
int GetActiveLevel();
double GetWTime();
double GetWTick();

class Mutex
{
public:
#if defined(_OPENMP) && defined(KVS_ENABLE_OPENMP)
    typedef omp_lock_t Handler;
#else
    typedef void* Handler;
#endif

private:
    Handler m_handler; ///< mutex handler

public:
    Mutex();
    virtual ~Mutex();

    const Handler& handler() const { return m_handler; }

    void lock();
    void unlock();
    bool tryLock();
};

} // end of namespace OpenMP

} // end of namespace kvs
