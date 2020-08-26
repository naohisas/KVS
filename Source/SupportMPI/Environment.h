/*****************************************************************************/
/**
 *  @file   Environment.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once


namespace kvs
{

namespace mpi
{

class Environment
{
public:
    Environment( int argc, char** argv );
    ~Environment();
};

} // end of namespace mpi

} // end of namespace kvs
