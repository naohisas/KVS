/*****************************************************************************/
/**
 *  @file   Argument.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Argument.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <kvs/CommandLine>


/*===========================================================================*/
/**
 *  @brief  Argument class.
 */
/*===========================================================================*/
class Argument : public kvs::CommandLine
{
public:

    Argument( int argc, char** argv );
};

#endif // __ARGUMENT_H__
