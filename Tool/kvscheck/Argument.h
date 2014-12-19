/****************************************************************************/
/**
 *  @file Argument.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Argument.h 625 2010-10-01 07:39:27Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVSCHECK__ARGUMENT_H_INCLUDE
#define KVSCHECK__ARGUMENT_H_INCLUDE

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

#endif // KVSCHECK__ARGUMENT_H_INCLUDE
