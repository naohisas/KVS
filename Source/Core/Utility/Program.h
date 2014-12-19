/*****************************************************************************/
/**
 *  @file   Program.h
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
#ifndef KVS__PROGRAM_H_INCLUDE
#define KVS__PROGRAM_H_INCLUDE

#include "Noncopyable.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Program class.
 */
/*===========================================================================*/
class Program : private kvs::Noncopyable
{
public:
    int start( int argc, char** argv );

private:
    virtual int exec( int argc, char** argv ) = 0;
};

} // end of namespace kvs

#endif // KVS__PROGRAM_H_INCLUDE
