/*****************************************************************************/
/**
 *  @file   Version.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <iostream>
#include <kvs/Program>
#include <kvs/Version>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  KVS version checker class.
 */
/*===========================================================================*/
class Version : public kvs::Program
{
    int exec( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Executes the checker program.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return 0 if the process is done sucessfully
 */
/*===========================================================================*/
inline int Version::exec( int argc, char** argv )
{
    const std::string kvs_version = kvs::Version::Name();
    std::cout << "KVS version : " << kvs_version << std::endl;
    return 0;
}

} // end of namespace kvscheck
