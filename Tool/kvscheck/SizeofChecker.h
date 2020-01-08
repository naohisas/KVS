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
#pragma once
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
    template <typename T> size_t sizeOf() const { return sizeof(T); }
    friend std::ostream& operator << ( std::ostream& os, const SizeofChecker& checker );
};

} // end of namespace kvscheck
