/****************************************************************************/
/**
 *  @file GLUT.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GLUT.h 1104 2012-04-21 06:00:16Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__GLUT__GLUT_H_INCLUDE
#define KVS__GLUT__GLUT_H_INCLUDE

#include <kvs/Platform>
#include <kvs/String>
#include <kvs/Compiler>
#include <kvs/OpenGL>

#if defined( KVS_PLATFORM_MACOSX )
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Returns GLUT description
 *  @return description
 */
/*===========================================================================*/
inline std::string Description()
{
    const std::string description( "GLUT - The OpenGL Utility Toolkit" );
    return description;
}

/*===========================================================================*/
/**
 *  @brief  Returns GLUT version.
 *  @return GLUT version
 */
/*===========================================================================*/
inline std::string Version()
{
    const std::string version = kvs::String::ToString( GLUT_API_VERSION );
    return version;
}

} // end of namespace glut

} // end of namespace kvs

#endif // KVS_SUPPORT_GLUT_GLUT_H_INCLUDE
