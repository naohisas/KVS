/*****************************************************************************/
/**
 *  @file   FragmentShader.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FragmentShader.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__FRAGMENT_SHADER_H_INCLUDE
#define KVS__FRAGMENT_SHADER_H_INCLUDE

#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  Fragment shader class.
 */
/*===========================================================================*/
class FragmentShader : public kvs::ShaderObject
{
public:

    typedef kvs::ShaderObject SuperClass;

public:

    FragmentShader();
    FragmentShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__FRAGMENT_SHADER_H_INCLUDE
