/****************************************************************************/
/**
 *  @file   WriteVCProject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteVCXProject.h 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSMAKE__WRITE_VCX_PROJECT_H_INCLUDE
#define KVSMAKE__WRITE_VCX_PROJECT_H_INCLUDE

#include <string>


namespace kvsmake
{

bool WriteVCXProject( const std::string& project_name );

} // end of namespace kvsmake

#endif // KVSMAKE__WRITE_VCX_PROJECT_H_INCLUDE
