/*****************************************************************************/
/**
 *  @file   MPI.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

// Deactivate C++ wrappers
#ifndef OMPI_SKIP_MPICXX
#define OMPI_SKIP_MPICXX 1
#define __KVS_MPI_SKIP_OMPI__
#endif

#ifndef MPICH_SKIP_MPICXX
#define MPICH_SKIP_MPICXX 1
#define __KVS_MPI_SKIP_MPICH__
#endif

#ifndef MPI_NO_CPPBIND
#define MPI_NO_CPPBIND 1
#define __KVS_MPI_SKIP_SGI__
#endif

// Include MPI header
#include <mpi.h>

// Activate C++ wrappers
#ifdef __KVS_MPI_SKIP_OMPI__
#undef OMPI_SKIP_MPICXX
#undef __KVS_MPI_SKIP_OMPI__
#endif

#ifdef __KVS_MPI_SKIP_MPICH__
#undef MPICH_SKIP_MPICXX
#undef __KVS_MPI_SKIP_MPICH__
#endif

#ifdef __KVS_MPI_SKIP_SGI__
#undef MPI_NO_CPPBIND
#undef __KVS_MPI_SKIP_SGI__
#endif

#include <kvs/String>
#include "MPICall.h"


namespace kvs
{

namespace mpi
{

inline const std::string Description()
{
    const std::string description("MPI - Message Passing Interface");
    return description;
}

inline const std::string Version()
{
    const int major_version = MPI_VERSION;
    const int minor_version = MPI_SUBVERSION;

    const std::string version(
        kvs::String::From( major_version ) + "." +
        kvs::String::From( minor_version ) );
    return version;
}

} // end of namespace mpi

} // end of namespace kvs
