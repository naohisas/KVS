/*****************************************************************************/
/**
 *  @file   MinMaxChecker.cpp
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
#include "MinMaxChecker.h"
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
std::ostream& operator << ( std::ostream& os, const MinMaxChecker& checker )
{
    os << "Min/Max value range:" << std::endl;
    os << "  char   = [ "
       << static_cast<int>( checker.minValueOf<char>() ) << ", "
       << static_cast<int>( checker.maxValueOf<char>() ) << " ]" << std::endl;
    os << "  uchar  = [ "
       << static_cast<unsigned int>( checker.minValueOf<unsigned char>() ) << ", "
       << static_cast<unsigned int>( checker.maxValueOf<unsigned char>() ) << " ]" << std::endl;
    os << "  short  = [ "
       << checker.minValueOf<short>() << ", "
       << checker.maxValueOf<short>() << " ]" << std::endl;
    os << "  ushort = [ "
       << checker.minValueOf<unsigned short>() << ", "
       << checker.maxValueOf<unsigned short>() << " ]" << std::endl;
    os << "  int    = [ "
       << checker.minValueOf<int>() << ", "
       << checker.maxValueOf<int>() << " ]" << std::endl;
    os << "  uint   = [ "
       << checker.minValueOf<unsigned int>() << ", "
       << checker.maxValueOf<unsigned int>() << " ]" << std::endl;
    os << "  long   = [ "
       << checker.minValueOf<long>() << ", "
       << checker.maxValueOf<long>() << " ]" << std::endl;
    os << "  ulong  = [ "
       << checker.minValueOf<unsigned long>() << ", "
       << checker.maxValueOf<unsigned long>() << " ]" << std::endl;
    os << "  float  = [ "
       << checker.minValueOf<float>() << ", "
       << checker.maxValueOf<float>() << " ]" << std::endl;
    os << "  double = [ "
       << checker.minValueOf<double>() << ", "
       << checker.maxValueOf<double>() << " ]" << std::endl;
    os << "  size_t = [ "
       << checker.minValueOf<size_t>() << ", "
       << checker.maxValueOf<size_t>() << " ]";

    return os;
}

} // end of namespace kvscheck
