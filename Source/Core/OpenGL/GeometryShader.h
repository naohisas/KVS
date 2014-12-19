/*****************************************************************************/
/**
 *  @file   GeometryShader.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GeometryShader.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__GEOMETRY_SHADER_H_INCLUDE
#define KVS__GEOMETRY_SHADER_H_INCLUDE

#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  Geometry shader class.
 */
/*===========================================================================*/
class GeometryShader : public kvs::ShaderObject
{
public:

    typedef kvs::ShaderObject SuperClass;

public:

    GeometryShader();
    GeometryShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__GEOMETRY_SHADER_H_INCLUDE
