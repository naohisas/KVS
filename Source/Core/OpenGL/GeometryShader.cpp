/*****************************************************************************/
/**
 *  @file   GeometryShader.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GeometryShader.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "GeometryShader.h"

#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER 0x8DD9
#endif


namespace kvs
{

GeometryShader::GeometryShader():
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
}

GeometryShader::GeometryShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}

} // end of namespace kvs
