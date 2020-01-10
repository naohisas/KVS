/*****************************************************************************/
/**
 *  @file   MinMaxChecker.h
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
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <kvs/Value>
#include <kvs/Program>


namespace kvscheck
{

class MinMax : public kvs::Program
{
public:
    int exec( int argc, char** argv );
};

#define KVSCHECK_PRINT_MINMAX( os, type )                               \
    os << std::left << std::setw(14) << #type << " : [ ";               \
    if ( std::string(#type) == "char" )                                 \
    {                                                                   \
        os << static_cast<int>( kvs::Value<type>::Min() ) << ", ";      \
        os << static_cast<int>( kvs::Value<type>::Max() ) << " ]";      \
    }                                                                   \
    else if ( std::string(#type) == "unsigned char" )                     \
    {                                                                   \
        os << static_cast<unsigned int>( kvs::Value<type>::Min() ) << ", "; \
        os << static_cast<unsigned int>( kvs::Value<type>::Max() ) << " ]"; \
    }                                                                   \
    else                                                                \
    {                                                                   \
        os << kvs::Value<type>::Min() << ", ";                          \
        os << kvs::Value<type>::Max() << " ]";                          \
    }                                                                   \
    os << std::endl;

inline int MinMax::exec( int argc, char** argv )
{
    KVSCHECK_PRINT_MINMAX( std::cout, char );
    KVSCHECK_PRINT_MINMAX( std::cout, unsigned char );
    KVSCHECK_PRINT_MINMAX( std::cout, short );
    KVSCHECK_PRINT_MINMAX( std::cout, unsigned short );
    KVSCHECK_PRINT_MINMAX( std::cout, int );
    KVSCHECK_PRINT_MINMAX( std::cout, unsigned int );
    KVSCHECK_PRINT_MINMAX( std::cout, long );
    KVSCHECK_PRINT_MINMAX( std::cout, unsigned long );
    KVSCHECK_PRINT_MINMAX( std::cout, float );
    KVSCHECK_PRINT_MINMAX( std::cout, double );
    return 0;
}

#undef KVSCHECK_PRINT_MINMAX

/*==========================================================================*/
/**
 *  Min/Max value checker class.
 */
/*==========================================================================*/
class MinMaxChecker
{
public:
    template <typename T> const T minValueOf() const { return kvs::Value<T>::Min(); }
    template <typename T> const T maxValueOf() const { return kvs::Value<T>::Max(); }
    friend std::ostream& operator << ( std::ostream& os, const MinMaxChecker& checker );
};

} // end of namespace kvscheck
