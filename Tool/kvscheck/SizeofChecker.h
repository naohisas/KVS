/*****************************************************************************/
/**
 *  @file   SizeofChecker.h
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
#ifndef KVSCHECK__SIZEOF_CHECKER_H_INCLUDE
#define KVSCHECK__SIZEOF_CHECKER_H_INCLUDE

#include <iostream>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  'sizeof' checker class.
 */
/*==========================================================================*/
class SizeofChecker
{
public:

    template <typename T> size_t sizeOf() const;
    friend std::ostream& operator << ( std::ostream& os, const SizeofChecker& checker );
};

/*===========================================================================*/
/**
 *  @brief  Returns size of specified type.
 *  @return size in byte
 */
/*===========================================================================*/
template <typename T>
inline size_t SizeofChecker::sizeOf() const
{
    return sizeof( T );
}

} // end of namespace kvscheck

#endif // KVSCHECK__SIZEOF_CHECKER_H_INCLUDE
