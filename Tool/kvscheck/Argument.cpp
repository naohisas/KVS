/****************************************************************************/
/**
 *  @file Argument.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Argument.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/****************************************************************************/
#include "Argument.h"
#include "CommandName.h"


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvs::CommandLine( argc, argv, kvscheck::CommandName )
{
    addHelpOption();
    addOption("version", "Output KVS version. (optional)");
    addOption("platform", "Output platforma information. (optional)");
    addOption("compiler", "Output compiler information. (optional)");
    addOption("sizeof", "Output 'sizeof' information. (optional)");
    addOption("support", "Output supported library information. (optional)");
    addOption("minmax", "Output min/max information. (optional)");
    addOption("opengl", "Output OpenGL information. (optional)");
    addOption("extension", "Output OpenGL extension information. (optional)");
    addOption("file", "Output file information. (optional)");
    addValue("input value", false);
}

} // end of namespace kvscheck
