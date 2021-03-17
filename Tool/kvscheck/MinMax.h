/*****************************************************************************/
/**
 *  @file   MinMax.h
 *  @author Naohisa Sakamoto
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

/*===========================================================================*/
/**
 *  @brief  Min/max value checker class.
 */
/*===========================================================================*/
class MinMax : public kvs::Program
{
    int exec()
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
};

#undef KVSCHECK_PRINT_MINMAX

} // end of namespace kvscheck
