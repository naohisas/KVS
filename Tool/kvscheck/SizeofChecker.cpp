/*****************************************************************************/
/**
 *  @file   SizeofChecker.cpp
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
#include "SizeofChecker.h"
#include <iostream>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  @brief  Output platform information.
 *  @param  os [in] output stream
 *  @param  checker [in] sizeof checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const SizeofChecker& checker )
{
    os << "Size of a type :" << std::endl;
    os << "  sizeof( char )   = " << checker.sizeOf<char>() << " bytes" << std::endl;
    os << "  sizeof( short )  = " << checker.sizeOf<short>() << " bytes" << std::endl;
    os << "  sizeof( int )    = " << checker.sizeOf<int>() << " bytes" << std::endl;
    os << "  sizeof( long )   = " << checker.sizeOf<long>() << " bytes" << std::endl;
    os << "  sizeof( float )  = " << checker.sizeOf<float>() << " bytes" << std::endl;
    os << "  sizeof( double ) = " << checker.sizeOf<double>() << " bytes" << std::endl;
    os << "  sizeof( size_t ) = " << checker.sizeOf<size_t>() << " bytes" << std::endl;
    os << "  sizeof( void* )  = " << checker.sizeOf<void*>() << " bytes";
    return os;
}

} // end of namespace kvscheck
