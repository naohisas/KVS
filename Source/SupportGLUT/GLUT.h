/****************************************************************************/
/**
 *  @file   GLUT.h
 *  @author Naohisa Sakamoto
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
    const std::string version = kvs::String::From( GLUT_API_VERSION );
    return version;
}

} // end of namespace glut

} // end of namespace kvs

#endif // KVS_SUPPORT_GLUT_GLUT_H_INCLUDE
