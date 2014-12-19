/*****************************************************************************/
/**
 *  @file   VertexShader.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VertexShader.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__VERTEX_SHADER_H_INCLUDE
#define KVS__VERTEX_SHADER_H_INCLUDE

#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  Vertex shader class.
 */
/*===========================================================================*/
class VertexShader : public kvs::ShaderObject
{
public:

    typedef kvs::ShaderObject SuperClass;

public:

    VertexShader();
    VertexShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__VERTEX_SHADER_H_INCLUDE
