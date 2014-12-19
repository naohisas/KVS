/****************************************************************************/
/**
 *  @file SystemInformation.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SystemInformation.h 1334 2012-10-18 13:47:59Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SYSTEM_INFORMATION_H_INCLUDE
#define KVS__SYSTEM_INFORMATION_H_INCLUDE

#include <cstdio>


namespace kvs
{

/*==========================================================================*/
/**
 *  System information class.
 */
/*==========================================================================*/
class SystemInformation
{
public:
    static size_t NumberOfProcessors();
    static size_t TotalMemorySize();
    static size_t FreeMemorySize();

private:
    SystemInformation();
};

} // end of namespace kvs

#endif // KVS__SYSTEM_INFORMATION_H_INCLUDE
