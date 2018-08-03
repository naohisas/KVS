#pragma once
#include <kvs/Compiler>


#if defined(_OPENMP) && defined(KVS_ENABLE_OPENMP)
#include <omp.h>
#if defined(KVS_COMPILER_VC)
#define KVS_OMP_PRAGMA(x) __pragma(x)
#else
#define KVS_OMP_PRAGMA(x) _Pragma(#x)
#endif
#define KVS_OMP(directive) KVS_OMP_PRAGMA(omp directive)

// Worksharing constructs
#define KVS_OMP_FOR(clauses) KVS_OMP(for clauses)
#define KVS_OMP_SECTIONS(clauses) KVS_OMP(sections clauses)
#define KVS_OMP_SECTION KVS_OMP(section)
#define KVS_OMP_SINGLE(clauses) KVS_OMP(single clauses)

// Master and synchronization constructs
#define KVS_OMP_MASTER KVS_OMP(master)
#define KVS_OMP_CRITICAL(name) KVS_OMP(critical name)
#define KVS_OMP_BARRIER KVS_OMP(barrier)
#define KVS_OMP_TASKWAIT KVS_OMP(taskwait)
#define KVS_OMP_ATOMIC KVS_OMP(atomic)
#define KVS_OMP_FLUSH(list) KVS_OMP(flush list)
#define KVS_OMP_ORDERED KVS_OMP(ordered)

// Combined parallel worksharing constructs
#define KVS_OMP_PARALLEL(clauses) KVS_OMP_PRAGMA(omp parallel clauses)
#define KVS_OMP_PARALLEL_FOR(clauses) KVS_OMP_PARALLEL(for clauses)
#define KVS_OMP_PARALLEL_SECTIONS(clauses) KVS_OMP_PARALLEL(sections clauses)

// Task construct
#define KVS_OMP_TASK(clauses) KVS_OMP(task clauses)

// Directives
#define KVS_OMP_THREADPRIVATE(list) KVS_OMP(threadprivate list)

#else
#define KVS_OMP_PRAGMA(x)
#define KVS_OMP(directive)
#define KVS_OMP_FOR(clauses)
#define KVS_OMP_SECTIONS(clauses)
#define KVS_OMP_SECTION
#define KVS_OMP_SINGLE(clauses)
#define KVS_OMP_MASTER
#define KVS_OMP_CRITICAL(name)
#define KVS_OMP_BARRIER
#define KVS_OMP_TASKWAIT
#define KVS_OMP_ATOMIC
#define KVS_OMP_FLUSH(list)
#define KVS_OMP_ORDERED
#define KVS_OMP_PARALLEL(clauses)
#define KVS_OMP_PARALLEL_FOR(clauses)
#define KVS_OMP_PARALLEL_SECTIONS(clauses)
#define KVS_OMP_TASK(directive)
#define KVS_OMP_THREADPRIVATE(list)
#endif
