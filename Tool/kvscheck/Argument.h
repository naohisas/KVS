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
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <kvs/CommandLine>


namespace kvscheck
{

/*==========================================================================*/
/**
 *  Argument class.
 */
/*==========================================================================*/
class Argument : public kvs::CommandLine
{
public:
    Argument( int argc, char** argv );
};

} // end of namespace kvscheck
