/*****************************************************************************/
/**
 *  @file   VertexShader.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VertexShader.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "VertexShader.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new VertexShader class.
 */
/*===========================================================================*/
VertexShader::VertexShader():
    kvs::ShaderObject( GL_VERTEX_SHADER )
{
}

/*===========================================================================*/
/**
 *  Construct a new VertexShader class.
 *  @param  source [in] shader source code
 */
/*===========================================================================*/
VertexShader::VertexShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_VERTEX_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}


} // end of namespace kvs
