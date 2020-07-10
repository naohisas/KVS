/****************************************************************************/
/**
 *  @file   Makefile.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteMakefile.h 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
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
