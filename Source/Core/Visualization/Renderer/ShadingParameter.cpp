/****************************************************************************/
/**
 *  @file ShadingParameter.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ShadingParameter.cpp 1113 2012-04-25 13:45:42Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "ShadingParameter.h"
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>


namespace kvs
{

ShadingParameter::ShadingParameter()
{
    m_Ka = 0;
    m_Kd = 0;
    m_Ks = 0;
    m_S = 0;
    m_type = UnknownShading;
}

ShadingParameter::ShadingParameter( const kvs::Shader::Base& shader )
{
    switch ( shader.type() )
    {
    case kvs::Shader::LambertShading:
        *this = Lambert( shader.Ka, shader.Kd );
        break;
    case kvs::Shader::PhongShading:
        *this = Phong( shader.Ka, shader.Kd, shader.Ks, shader.S );
        break;
    case kvs::Shader::BlinnPhongShading:
        *this = BlinnPhong( shader.Ka, shader.Kd, shader.Ks, shader.S );
        break;
    default:
        /* NO SHADING */
        break;
    }
}

ShadingParameter ShadingParameter::Lambert( float ka, float kd )
{
    ShadingParameter param;
    param.m_Ka = ka;
    param.m_Kd = kd;
    param.m_Ks = 0;
    param.m_S = 0;
    param.m_type = LambertShading;
    return param;
}

ShadingParameter ShadingParameter::Phong( float ka, float kd, float ks, float s )
{
    ShadingParameter param;
    param.m_Ka = ka;
    param.m_Kd = kd;
    param.m_Ks = ks;
    param.m_S = s;
    param.m_type = PhongShading;
    return param;
}

ShadingParameter ShadingParameter::BlinnPhong( float ka, float kd, float ks, float s )
{
    ShadingParameter param;
    param.m_Ka = ka;
    param.m_Kd = kd;
    param.m_Ks = ks;
    param.m_S = s;
    param.m_type = BlinnPhongShading;
    return param;
}

void ShadingParameter::apply( ProgramObject& program, const std::string& var_name ) const
{
    program.setUniform( ( var_name + ".Ka" ).c_str(), m_Ka );
    program.setUniform( ( var_name + ".Kd" ).c_str(), m_Kd );
    if ( m_type == PhongShading || m_type == BlinnPhongShading )
    {
        program.setUniform( ( var_name + ".Ks" ).c_str(), m_Ks );
        program.setUniform( ( var_name + ".S" ).c_str(),  m_S );
    }
}

void ShadingParameter::defineMacro( ShaderSource& frag ) const
{
    switch ( m_type )
    {
    case LambertShading:
        frag.define( "ENABLE_LAMBERT_SHADING" );
        break;
    case PhongShading:
        frag.define( "ENABLE_PHONG_SHADING" );
        break;
    case BlinnPhongShading:
        frag.define( "ENABLE_BLINN_PHONG_SHADING" );
        break;
    default:
        /* NO SHADING */
        break;
    }
}

} // end of namespace kvs
