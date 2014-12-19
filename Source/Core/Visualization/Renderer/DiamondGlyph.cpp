/*****************************************************************************/
/**
 *  @file   DiamondGlyph.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DiamondGlyph.cpp 1634 2013-09-06 08:55:47Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "DiamondGlyph.h"
#include <kvs/OpenGL>


namespace
{

const kvs::UInt32 Connections[24] =
{
    4, 0, 1,
    4, 1, 2,
    4, 2, 3,
    4, 3, 0,
    5, 1, 0,
    5, 2, 1,
    5, 3, 2,
    5, 0, 3
};

const kvs::Real32 Vertices[18] =
{
     0.5f,  0.0f,  0.0f,
     0.0f,  0.0f, -0.5f,
    -0.5f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.5f,
     0.0f,  0.5f,  0.0f,
     0.0f, -0.5f,  0.0f
};

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new DiamondGlyph class.
 */
/*===========================================================================*/
DiamondGlyph::DiamondGlyph():
    kvs::GlyphBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new DiamondGlyph class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
DiamondGlyph::DiamondGlyph( const kvs::VolumeObjectBase* volume ):
    kvs::GlyphBase()
{
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new DiamondGlyph class.
 *  @param  volume [in] pointer to the volume object
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
DiamondGlyph::DiamondGlyph(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::GlyphBase()
{
    BaseClass::setTransferFunction( transfer_function );
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Diamond glyph class.
 */
/*===========================================================================*/
DiamondGlyph::~DiamondGlyph()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] pointer to the volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void DiamondGlyph::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume ) { kvsMessageError("Input object is not volume dat."); return; }
    if ( m_volume != volume ) { this->attach_volume( volume ); }

    BaseClass::startTimer();

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glEnable( GL_DEPTH_TEST );
    this->initialize();
    this->draw();
    glDisable( GL_DEPTH_TEST );

    glPopAttrib();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates a diamond glyph.
 *  @param  object [in] pointer to the input object (volume object)
 *  @return pointer to the created glyph object
 */
/*===========================================================================*/
void DiamondGlyph::attach_volume( const kvs::VolumeObjectBase* volume )
{
    m_volume = volume;

    BaseClass::calculateCoords( volume );

    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::Int8 ) )
    {
        BaseClass::calculateSizes<kvs::Int8>( volume );
        BaseClass::calculateDirections<kvs::Int8>( volume );
        BaseClass::calculateColors<kvs::Int8>( volume );
        BaseClass::calculateOpacities<kvs::Int8>( volume );
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        BaseClass::calculateSizes<kvs::Int16>( volume );
        BaseClass::calculateDirections<kvs::Int16>( volume );
        BaseClass::calculateColors<kvs::Int16>( volume );
        BaseClass::calculateOpacities<kvs::Int16>( volume );
    }
    else if ( type == typeid( kvs::Int32 ) )
    {
        BaseClass::calculateSizes<kvs::Int32>( volume );
        BaseClass::calculateDirections<kvs::Int32>( volume );
        BaseClass::calculateColors<kvs::Int32>( volume );
        BaseClass::calculateOpacities<kvs::Int32>( volume );
    }
    else if ( type == typeid( kvs::Int64 ) )
    {
        BaseClass::calculateSizes<kvs::Int64>( volume );
        BaseClass::calculateDirections<kvs::Int64>( volume );
        BaseClass::calculateColors<kvs::Int64>( volume );
        BaseClass::calculateOpacities<kvs::Int64>( volume );
    }
    else if ( type == typeid( kvs::UInt8  ) )
    {
        BaseClass::calculateSizes<kvs::UInt8>( volume );
        BaseClass::calculateDirections<kvs::UInt8>( volume );
        BaseClass::calculateColors<kvs::UInt8>( volume );
        BaseClass::calculateOpacities<kvs::UInt8>( volume );
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        BaseClass::calculateSizes<kvs::UInt16>( volume );
        BaseClass::calculateDirections<kvs::UInt16>( volume );
        BaseClass::calculateColors<kvs::UInt16>( volume );
        BaseClass::calculateOpacities<kvs::UInt16>( volume );
    }
    else if ( type == typeid( kvs::UInt32 ) )
    {
        BaseClass::calculateSizes<kvs::UInt32>( volume );
        BaseClass::calculateDirections<kvs::UInt32>( volume );
        BaseClass::calculateColors<kvs::UInt32>( volume );
        BaseClass::calculateOpacities<kvs::UInt32>( volume );
    }
    else if ( type == typeid( kvs::UInt64 ) )
    {
        BaseClass::calculateSizes<kvs::UInt64>( volume );
        BaseClass::calculateDirections<kvs::UInt64>( volume );
        BaseClass::calculateColors<kvs::UInt64>( volume );
        BaseClass::calculateOpacities<kvs::UInt64>( volume );
    }
    else if ( type == typeid( kvs::Real32 ) )
    {
        BaseClass::calculateSizes<kvs::Real32>( volume );
        BaseClass::calculateDirections<kvs::Real32>( volume );
        BaseClass::calculateColors<kvs::Real32>( volume );
        BaseClass::calculateOpacities<kvs::Real32>( volume );
    }
    else if ( type == typeid( kvs::Real64 ) )
    {
        BaseClass::calculateSizes<kvs::Real64>( volume );
        BaseClass::calculateDirections<kvs::Real64>( volume );
        BaseClass::calculateColors<kvs::Real64>( volume );
        BaseClass::calculateOpacities<kvs::Real64>( volume );
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the diamond glyph.
 */
/*===========================================================================*/
void DiamondGlyph::draw()
{
    const size_t npoints = BaseClass::coords().size() / 3;
    if ( BaseClass::directions().size() == 0 )
    {
        for ( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( BaseClass::coords().data() + index );
            const kvs::Real32 size = BaseClass::sizes()[i];
            const kvs::RGBColor color( BaseClass::colors().data() + index );
            const kvs::UInt8 opacity = BaseClass::opacities()[i];
            glPushMatrix();
            {
                BaseClass::transform( position, size );
                this->draw_element( color, opacity );
            }
            glPopMatrix();
        }
    }
    else
    {
        for( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( BaseClass::coords().data() + index );
            const kvs::Vector3f direction( BaseClass::directions().data() + index );
            const kvs::Real32 size = BaseClass::sizes()[i];
            const kvs::RGBColor color( BaseClass::colors().data() + index );
            const kvs::UInt8 opacity = BaseClass::opacities()[i];
            glPushMatrix();
            {
                BaseClass::transform( position, direction, size );
                this->draw_element( color, opacity );
            }
            glPopMatrix();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the glyph element.
 *  @param  color [in] color value
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void DiamondGlyph::draw_element( const kvs::RGBColor& color, const kvs::UInt8 opacity )
{
    glBegin( GL_TRIANGLES );
    {
        glColor4ub( color.r(), color.g(), color.b(), opacity );

        for( size_t i = 0, index = 0; i < 8; i++, index += 3 )
        {
            const kvs::UInt32 offset0 = ::Connections[index] * 3;
            const kvs::UInt32 offset1 = ::Connections[index+1] * 3;
            const kvs::UInt32 offset2 = ::Connections[index+2] * 3;

            const kvs::Vector3f v0( ::Vertices + offset0 );
            const kvs::Vector3f v1( ::Vertices + offset1 );
            const kvs::Vector3f v2( ::Vertices + offset2 );
            const kvs::Vector3f v10 = v0 - v1;
            const kvs::Vector3f v12 = v2 - v1;
            const kvs::Vector3f n = v12.cross( v10 );
            glNormal3f( n.x(), n.y(), n.z() );
            glVertex3fv( ::Vertices + offset0 );
            glVertex3fv( ::Vertices + offset1 );
            glVertex3fv( ::Vertices + offset2 );
        }
    }
    glEnd();
}

/*===========================================================================*/
/**
 *  @brief  Initialize the modelview matrix.
 */
/*===========================================================================*/
void DiamondGlyph::initialize()
{
    glDisable( GL_LINE_SMOOTH );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glShadeModel( GL_SMOOTH );

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    if ( !BaseClass::isEnabledShading() )
    {
        glDisable( GL_NORMALIZE );
        glDisable( GL_LIGHTING );
    }
    else
    {
        glEnable( GL_NORMALIZE );
        glEnable( GL_LIGHTING );
    }
}

} // end of namespace kvs
