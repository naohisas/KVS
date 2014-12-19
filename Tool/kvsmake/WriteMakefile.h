/****************************************************************************/
/**
 *  @file   WriteMakefile.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteMakefile.h 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSMAKE__WRITE_MAKEFILE_H_INCLUDE
#define KVSMAKE__WRITE_MAKEFILE_H_INCLUDE

#include <string>


namespace kvsmake
{

bool WriteMakefile( const std::string& project_name );

}

#endif // KVSMAKE__WRITE_MAKEFILE_H_INCLUDE
