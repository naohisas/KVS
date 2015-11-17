/****************************************************************************/
/**
 *  @file Material.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Material.cpp 1103 2012-04-21 05:32:51Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Material.h"

#if KVS_ENABLE_DEPRECATED

#include <cstring>
#include <kvs/Vector4>
#include <kvs/RGBAColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Material class.
 */
/*===========================================================================*/
Material::Material( void )
{
    this->initialize();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Material class.
 *  @param  ambient [in] ambient color
 *  @param  diffuse [in] diffuse color
 *  @param  specular [in] specular color
 *  @param  shininess [in] shininess value
 *  @param  face [in] material face
 */
/*===========================================================================*/
Material::Material(
    const kvs::RGBAColor& ambient,
    const kvs::RGBAColor& diffuse,
    const kvs::RGBAColor& specular,
    const float shininess,
    const MaterialFace face )
{
    this->setAmbient( ambient );
    this->setDiffuse( diffuse );
    this->setSpecular( specular );
    this->setShininess( shininess );
    this->setFace( face );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Material class.
 *  @param  ambient [in] ambient color array
 *  @param  diffuse [in] diffuse color array
 *  @param  specular [in] specular color array
 *  @param  shininess [in] shininess value
 *  @param  face [in] material face
 */
/*===========================================================================*/
Material::Material(
    const float ambient[4],
    const float diffuse[4],
    const float specular[4],
    const float shininess,
    const MaterialFace face )
{
    this->setAmbient( ambient );
    this->setDiffuse( diffuse );
    this->setSpecular( specular );
    this->setShininess( shininess );
    this->setFace( face );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Material class.
 *  @param  ambi_r [in] red component of the ambient color
 *  @param  ambi_g [in] green component of the ambient color
 *  @param  ambi_b [in] blue component of the ambient color
 *  @param  ambi_a [in] alpha component of the ambient color
 *  @param  diff_r [in] red component of the diffuse color
 *  @param  diff_g [in] green component of the diffuse color
 *  @param  diff_b [in] blue component of the diffuse color
 *  @param  diff_a [in] alpha component of the diffuse color
 *  @param  spec_r [in] red component of the specular color
 *  @param  spec_g [in] green component of the specular color
 *  @param  spec_b [in] blue component of the specular color
 *  @param  spec_a [in] alpha component of the specular color
 *  @param  shininess [in] shininess value
 *  @param  face [in] material face
 */
/*===========================================================================*/
Material::Material(
    const float ambi_r, const float ambi_g, const float ambi_b, const float ambi_a,
    const float diff_r, const float diff_g, const float diff_b, const float diff_a,
    const float spec_r, const float spec_g, const float spec_b, const float spec_a,
    const float shininess,
    const MaterialFace face )
{
    this->setAmbient( ambi_r, ambi_g, ambi_b, ambi_a );
    this->setDiffuse( diff_r, diff_g, diff_b, diff_a );
    this->setSpecular( spec_r, spec_g, spec_b, spec_a );
    this->setShininess( shininess );
    this->setFace( face );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Material class.
 *  @param  material [in] material
 */
/*===========================================================================*/
Material::Material( const Material& material )
{
    *this = material;
}

/*===========================================================================*/
/**
 *  @brief  Destructs the material class.
 */
/*===========================================================================*/
Material::~Material( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Initiailize the material class.
 */
/*===========================================================================*/
void Material::initialize( void )
{
    m_ambient[0]  = m_ambient[1]  = m_ambient[2]  = m_ambient[3]  = 1.0f;
    m_diffuse[0]  = m_diffuse[1]  = m_diffuse[2]  = m_diffuse[3]  = 1.0f;
    m_specular[0] = m_specular[1] = m_specular[2] = m_specular[3] = 1.0f;
    m_shininess   = 100.0f;
    m_face = FrontAndBack;
}

/*===========================================================================*/
/**
 *  @brief  Clear the material class.
 */
/*===========================================================================*/
void Material::clear( void )
{
    m_ambient[0]  = m_ambient[1]  = m_ambient[2]  = m_ambient[3]  = 0.0f;
    m_diffuse[0]  = m_diffuse[1]  = m_diffuse[2]  = m_diffuse[3]  = 0.0f;
    m_specular[0] = m_specular[1] = m_specular[2] = m_specular[3] = 0.0f;
    m_shininess   = 0.0f;
    m_face = FrontAndBack;
}

/*===========================================================================*/
/**
 *  @brief  '=' operator.
 *  @param  material [in] material
 */
/*===========================================================================*/
Material& Material::operator = ( const Material& material )
{
    size_t byte_size = sizeof(float) * 4;

    memcpy( m_ambient,  material.m_ambient,  byte_size );
    memcpy( m_diffuse,  material.m_diffuse,  byte_size );
    memcpy( m_specular, material.m_specular, byte_size );
    m_shininess = material.m_shininess;
    m_face      = material.m_face;

    return( *this );
}

/*===========================================================================*/
/**
 *  @brief  Returns the ambient color.
 *  @return ambient color
 */
/*===========================================================================*/
const kvs::Vector4f Material::ambient( void ) const
{
    return( kvs::Vector4f( m_ambient ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the diffuse color.
 *  @return diffuse color
 */
/*===========================================================================*/
const kvs::Vector4f Material::diffuse( void ) const
{
    return( kvs::Vector4f( m_diffuse ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the specular color.
 *  @return specular color
 */
/*===========================================================================*/
const kvs::Vector4f Material::specular( void ) const
{
    return( kvs::Vector4f( m_specular ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the shininess value.
 *  @return shininess value
 */
/*===========================================================================*/
const float Material::shininess( void ) const
{
    return( m_shininess );
}

/*===========================================================================*/
/**
 *  @brief  Sets a material face.
 *  @param  face [in] material face
 */
/*===========================================================================*/
void Material::setFace( const MaterialFace face )
{
    m_face = face;
}

/*===========================================================================*/
/**
 *  @brief  Sets an ambient color.
 *  @param  r [in] red [0-1]
 *  @param  g [in] green [0-1]
 *  @param  b [in] blue [0-1]
 *  @param  a [in] alpha [0-1]
 */
/*===========================================================================*/
void Material::setAmbient( const float r, const float g, const float b, const float a )
{
    m_ambient[0] = r;
    m_ambient[1] = g;
    m_ambient[2] = b;
    m_ambient[3] = a;
}

/*===========================================================================*/
/**
 *  @brief  Sets an ambient color array.
 *  @param  ambient [in] ambient color array
 */
/*===========================================================================*/
void Material::setAmbient( const float ambient[4] )
{
    memcpy( m_ambient, ambient, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets an ambient color.
 *  @param  ambient [in] ambient color
 */
/*===========================================================================*/
void Material::setAmbient( const kvs::RGBAColor& ambient )
{
    const float ambi[4] = {
        static_cast<float>(ambient.r()) / 255.0f,
        static_cast<float>(ambient.g()) / 255.0f,
        static_cast<float>(ambient.b()) / 255.0f,
        static_cast<float>(ambient.a()) };

    memcpy( m_ambient, ambi, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a diffuse color.
 *  @param  r [in] red [0-1]
 *  @param  g [in] green [0-1]
 *  @param  b [in] blue [0-1]
 *  @param  a [in] alpha [0-1]
 */
/*===========================================================================*/
void Material::setDiffuse( const float r, const float g, const float b, const float a )
{
    m_diffuse[0] = r;
    m_diffuse[1] = g;
    m_diffuse[2] = b;
    m_diffuse[3] = a;
}

/*===========================================================================*/
/**
 *  @brief  Sets a diffuse color array.
 *  @param  diffuse [in] diffuse color array
 */
/*===========================================================================*/
void Material::setDiffuse( const float diffuse[4] )
{
    memcpy( m_diffuse, diffuse, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a diffuse color.
 *  @param  diffuse [in] diffuse color
 */
/*===========================================================================*/
void Material::setDiffuse( const kvs::RGBAColor& diffuse )
{
    const float diff[4] = {
        static_cast<float>(diffuse.r()) / 255.0f,
        static_cast<float>(diffuse.g()) / 255.0f,
        static_cast<float>(diffuse.b()) / 255.0f,
        static_cast<float>(diffuse.a()) };

    memcpy( m_diffuse, diff, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a specular color.
 *  @param  r [in] red [0-1]
 *  @param  g [in] green [0-1]
 *  @param  b [in] blue [0-1]
 *  @param  a [in] alpha [0-1]
 */
/*===========================================================================*/
void Material::setSpecular( const float r, const float g, const float b, const float a )
{
    m_specular[0] = r;
    m_specular[1] = g;
    m_specular[2] = b;
    m_specular[3] = a;
}

/*===========================================================================*/
/**
 *  @brief  Sets a specular color array.
 *  @param  specular [in] specular color array
 */
/*===========================================================================*/
void Material::setSpecular( const float specular[4] )
{
    memcpy( m_specular, specular, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a specular color.
 *  @param  specular [in] specular color
 */
/*===========================================================================*/
void Material::setSpecular( const kvs::RGBAColor& specular )
{
    float spec[4] = { static_cast<float>(specular.r()) / 255.0f,
                      static_cast<float>(specular.g()) / 255.0f,
                      static_cast<float>(specular.b()) / 255.0f,
                      static_cast<float>(specular.a()) };

    memcpy( m_specular, spec, sizeof(float) * 4 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a shininiess value.
 *  @param  shininess [in] shininess value
 */
/*===========================================================================*/
void Material::setShininess( const float shininess )
{
    m_shininess = shininess;
}

/*===========================================================================*/
/**
 *  @brief  Applies the material.
 */
/*===========================================================================*/
void Material::apply( void )
{
    KVS_GL_CALL( glMaterialfv( (GLenum)m_face, GL_AMBIENT,   m_ambient ) );
    KVS_GL_CALL( glMaterialfv( (GLenum)m_face, GL_DIFFUSE,   m_diffuse ) );
    KVS_GL_CALL( glMaterialfv( (GLenum)m_face, GL_SPECULAR,  m_specular ) );
    KVS_GL_CALL( glMaterialf(  (GLenum)m_face, GL_SHININESS, m_shininess ) );
}

} // end of namespace kvs

#endif
