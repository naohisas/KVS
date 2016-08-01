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

} // end of namespace OpenMP

} // end of namespace kvs
