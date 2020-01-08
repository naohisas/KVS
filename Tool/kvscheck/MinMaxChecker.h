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
    template <typename T> const T minValueOf() const { return kvs::Value<T>::Min(); }
    template <typename T> const T maxValueOf() const { return kvs::Value<T>::Max(); }
    friend std::ostream& operator << ( std::ostream& os, const MinMaxChecker& checker );
};

} // end of namespace kvscheck
