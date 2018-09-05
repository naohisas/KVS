/*****************************************************************************/
/**
 *  @file   SystemInformation.h
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
#include <cstdio>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  System information class.
 */
/*===========================================================================*/
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
