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
#ifndef KVSCHECK__MINMAX_CHECKER_H_INCLUDE
#define KVSCHECK__MINMAX_CHECKER_H_INCLUDE

#include <iostream>
#include <kvs/Value>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  Min/Max value checker class.
 */
/*==========================================================================*/
class MinMaxChecker
{
public:

    template <typename T> const T minValueOf() const;
    template <typename T> const T maxValueOf() const;
    friend std::ostream& operator << ( std::ostream& os, const MinMaxChecker& checker );
};

/*===========================================================================*/
/**
 *  @brief  Returns minimum value of specified type.
 *  @return minimum value
 */
/*===========================================================================*/
template <typename T>
inline const T MinMaxChecker::minValueOf() const
{
    return( kvs::Value<T>::Min() );
}

/*===========================================================================*/
/**
 *  @brief  Returns maximum value of specified type.
 *  @return maximum value
 */
/*===========================================================================*/
template <typename T>
inline const T MinMaxChecker::maxValueOf() const
{
    return( kvs::Value<T>::Max() );
}

} // end of namespace kvscheck

#endif // KVSCHECK__MINMAX_CHECKER_H_INCLUDE
