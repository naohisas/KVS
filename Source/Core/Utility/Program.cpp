/*****************************************************************************/
/**
 *  @file   Program.cpp
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
#include "Program.h"
#include <exception>
#include <iostream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Trigger method for executing the 'exec' method within try-catch statement.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument value
 *  @return retur 0 if the process is done successfully
 */
/*===========================================================================*/
int Program::start( int argc, char** argv )
{
    try
    {
        return this->exec( argc, argv );
    }
    catch ( const std::exception& err )
    {
        std::cerr << "An exception was thrown" << std::endl;
        std::cerr << err.what() << std::endl;
        throw;
    }
    catch ( ... )
    {
        std::cerr << "Unknown exception was thrown" << std::endl;
        throw;
    }
}

} // end of namespace kvs
