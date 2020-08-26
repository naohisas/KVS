/****************************************************************************/
/**
 *  @file   Makefile.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/Program>


namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  Makefile generator class.
 */
/*===========================================================================*/
class Makefile : public kvs::Program
{
private:
    std::string m_project_name; ///< project name
    bool m_use_mpi; ///< flag for using MPI compiler
    int exec( int argc, char** argv );
public:
    Makefile( const std::string& project_name, const bool use_mpi = false ):
        m_project_name( project_name ),
        m_use_mpi( use_mpi ) {}
};

}
