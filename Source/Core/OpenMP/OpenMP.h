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
void SetDynamic( int dthreads );
int GetDynamic();
void SetNested( int nested );
int GetNested();
void SetMaxActiveLevels( int max_levels );
int GetMaxActiveLevels();
int GetLevel();
int GetAncestorThreadNumber( int level );
int GetTeamSize( int level );
int GetActiveLevel();
//bool InFinal();
double GetWTime();
double GetWTick();

} // end of namespace OpenMP

} // end of namespace kvs
