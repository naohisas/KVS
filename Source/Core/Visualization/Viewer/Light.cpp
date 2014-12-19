/****************************************************************************/
/**
 *  @file Light.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Light.cpp 1799 2014-08-04 05:36:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Light.h"
#include <kvs/OpenGL>
#include <kvs/RGBAColor>
#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/Coordinate>


namespace kvs
{

/*==========================================================================*/
/**
 *  Set the lighting model as a local-viewer.
 *  @param flag [in] set flag
 */
/*==========================================================================*/
void Light::SetModelLocalViewer( bool flag )
{
    KVS_GL_CALL( glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, flag ) );
}

/*==========================================================================*/
/**
 *  Set the lighting model as a two-side.
 *  @param flag [in] set flag
 */
/*==========================================================================*/
void Light::SetModelTwoSide( bool flag )
{
    KVS_GL_CALL( glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, flag ) );
}

/*==========================================================================*/
/**
 *  Set the lighting model as a ambient light.
 *  @param ambient [in] ambient color
 */
/*==========================================================================*/
void Light::SetModelAmbient( float ambient[4] )
{
    KVS_GL_CALL( glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient ) );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param collision [in] collision dectection flag
 */
/*==========================================================================*/
Light::Light()
{
    this->initialize();
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Light::~Light()
{
}

/*==========================================================================*/
/**
 *  Initialize the member parameters.
 */
/*==========================================================================*/
void Light::initialize()
{
    m_id = GL_LIGHT0;
    m_transform_center.set( 0, 0, 0 );
    this->setXform( kvs::Xform::Translation( kvs::Vec3( 0, 0, 12 ) ) );
    this->saveXform();
    m_diffuse.set( 1.0, 1.0, 1.0 );
    m_ambient.set( 0.0, 0.0, 0.0 );
    m_specular.set( 1.0, 1.0, 1.0 );
}

/*==========================================================================*/
/**
 *  Set the light ID.
 *  @param id [in] light ID
 */
/*==========================================================================*/
void Light::setID( const unsigned int id )
{
    m_id = id;
}

/*==========================================================================*/
/**
 *  Set the light position.
 *  @param x [in] x coordinate value
 *  @param y [in] y coordinate value
 *  @param z [in] z coordinate value
 */
/*==========================================================================*/
void Light::setPosition( const float x, const float y, const float z )
{
    this->setPosition( kvs::Vec3( x, y, z ) );
}

/*==========================================================================*/
/**
 *  Set the light position.
 *  @param position [in] light position
 */
/*==========================================================================*/
void Light::setPosition( const kvs::Vec3& position )
{
    this->setXform( kvs::Xform::Translation( position ) );
}

/*==========================================================================*/
/**
 *  Set the diffuse color of the light.
 *  @param r [in] red component (0.0-1.0)
 *  @param g [in] green component (0.0-1.0)
 *  @param b [in] blue component (0.0-1.0)
 */
/*==========================================================================*/
void Light::setColor( const float r, const float g, const float b )
{
    this->setDiffuse( r, g, b );
}

/*==========================================================================*/
/**
 *  Set the diffuse color of the light.
 *  @param color [in] diffuse color
 */
/*==========================================================================*/
void Light::setColor( const kvs::RGBAColor& color )
{
    this->setDiffuse( color );
}

/*==========================================================================*/
/**
 *  Set the diffuse color of the light.
 *  @param r [in] red component (0.0-1.0)
 *  @param g [in] green component (0.0-1.0)
 *  @param b [in] blue component (0.0-1.0)
 */
/*==========================================================================*/
void Light::setDiffuse( const float r, const float g, const float b )
{
    m_diffuse.set( r, g, b );
}

/*==========================================================================*/
/**
 *  Set the diffuse color of the light.
 *  @param color [in] diffuse color
 */
/*==========================================================================*/
void Light::setDiffuse( const kvs::RGBAColor& color )
{
    m_diffuse = color.toVec3();
}

/*==========================================================================*/
/**
 *  Set the ambient color of the light.
 *  @param r [in] red component (0.0-1.0)
 *  @param g [in] green component (0.0-1.0)
 *  @param b [in] blue component (0.0-1.0)
 */
/*==========================================================================*/
void Light::setAmbient( const float r, const float g, const float b )
{
    m_ambient.set( r, g, b );
}

/*==========================================================================*/
/**
 *  Set the ambient color of the light.
 *  @param color [in] ambient color
 */
/*==========================================================================*/
void Light::setAmbient( const kvs::RGBAColor& color )
{
    m_ambient = color.toVec3();
}

/*==========================================================================*/
/**
 *  Set the specular color of the light.
 *  @param r [in] red component (0.0-1.0)
 *  @param g [in] green component (0.0-1.0)
 *  @param b [in] blue component (0.0-1.0)
 */
/*==========================================================================*/
void Light::setSpecular( const float r, const float g, const float b )
{
    m_specular.set( r, g, b );
}

/*==========================================================================*/
/**
 *  Set the specular color of the light.
 *  @param color [in] specular color
 */
/*==========================================================================*/
void Light::setSpecular( const kvs::RGBAColor& color )
{
    m_specular = color.toVec3();
}

/*==========================================================================*/
/**
 *  Get the light position.
 */
/*==========================================================================*/
const kvs::Vec3 Light::position() const
{
    return this->xform().translation();
}

/*==========================================================================*/
/**
 *  Update the light.
 */
/*==========================================================================*/
void Light::update( const kvs::Camera* camera )
{
    // World coordinate to camera coordinate.
    const kvs::Vec3 p = kvs::WorldCoordinate( this->position() ).toCameraCoordinate( camera ).position();

    const kvs::Vec4 position( p, 1.0f );
    const kvs::Vec4 diffuse( this->diffuse(), 1.0f );
    const kvs::Vec4 ambient( this->ambient(), 1.0f );
    const kvs::Vec4 specular( this->specular(), 1.0f );

    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();
    kvs::OpenGL::SetLight( m_id, GL_POSITION, (GLfloat*)&(position[0]) );
    kvs::OpenGL::SetLight( m_id, GL_DIFFUSE,  (GLfloat*)&(diffuse[0]) );
    kvs::OpenGL::SetLight( m_id, GL_AMBIENT,  (GLfloat*)&(ambient[0]) );
    kvs::OpenGL::SetLight( m_id, GL_SPECULAR, (GLfloat*)&(specular[0]) );
    kvs::OpenGL::PopMatrix();
}

/*==========================================================================*/
/**
 *  Turn on the light.
 */
/*==========================================================================*/
void Light::on() const
{
    kvs::OpenGL::Enable( m_id );
}

/*==========================================================================*/
/**
 *  Turn off the light.
 */
/*==========================================================================*/
void Light::off() const
{
    kvs::OpenGL::Disable( m_id );
}

/*==========================================================================*/
/**
 *  Test whether the light is enable or not.
 *  @return true, if the light is enable.
 */
/*==========================================================================*/
bool Light::isEnabled() const
{
    return kvs::OpenGL::IsEnabled( m_id );
}

/*===========================================================================*/
/**
 *  @brief  Resets the xform of the light.
 */
/*===========================================================================*/
void Light::resetXform()
{
    kvs::XformControl::resetXform();
    m_transform_center.set( 0, 0, 0 );
}

/*==========================================================================*/
/**
 *  Rotate the light.
 *  @param rotation [in] rotation matrix
 */
/*==========================================================================*/
void Light::rotate( const kvs::Mat3& rotation )
{
    const kvs::Vec3 t = m_transform_center;
    const kvs::Xform x = kvs::Xform::Translation( t )
                       * kvs::Xform::Rotation( rotation )
                       * kvs::Xform::Translation( -t );
    this->multiplyXform( x );
}

/*==========================================================================*/
/**
 *  Translate the light.
 *  @param translation [in] translation vector
 */
/*==========================================================================*/
void Light::translate( const kvs::Vec3& translation )
{
    this->multiplyXform( kvs::Xform::Translation( translation ) );
    m_transform_center += translation;
}

/*==========================================================================*/
/**
 *  Scale the light.
 *  @param scaling [in] scaling vector.
 */
/*==========================================================================*/
void Light::scale( const kvs::Vec3& scaling )
{
    this->multiplyXform( kvs::Xform::Scaling( scaling ) );
}

} // end of namespace kvs

