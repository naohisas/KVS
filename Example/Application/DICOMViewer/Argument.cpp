/*****************************************************************************/
/**
 *  @file   Argument.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Argument.cpp 1221 2012-06-18 10:51:25Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "Argument.h"


/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvs::CommandLine( argc, argv )
{
    addHelpOption();
    addOption("f", "Force reading DICOM files.");
    addValue("file or directory");
}
