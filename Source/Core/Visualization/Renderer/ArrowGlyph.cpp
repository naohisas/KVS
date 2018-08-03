/*****************************************************************************/
/**
 *  @file   ArrowGlyph.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ArrowGlyph.cpp 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ArrowGlyph.h"
#include <kvs/OpenGL>
#include <kvs/IgnoreUnusedVariable>

namespace
{

const kvs::Real32 LineVertices[12] =
{
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f,
    -0.2f, 0.8f, 0.0f,
     0.2f, 0.8f, 0.0f
};
const kvs::UInt32 LineConnections[6] =
{
    0, 1,
    0, 2,
    0, 3
};

const kvs::Vec3 ConeTranslation = kvs::Vec3( 0.0f, 0.0f, 0.7f );
inline void DrawCone( const kvs::Vec3& t )
{
    const GLdouble base = 0.15;
    const GLdouble top = 0.0;
    const GLdouble height = 0.3;
    const GLint slices = 20;
    const GLint stacks = 5;
    kvs::OpenGL::Translate( t );
    kvs::OpenGL::DrawCylinder( base, top, height, slices, stacks );
}

const kvs::Vec3 CylinderTranslation = kvs::Vec3( 0.0f, 0.0f, 0.0f );
inline void DrawCylinder( const kvs::Vec3& t )
{
    const GLdouble base = 0.07;
    const GLdouble top = 0.07;
    const GLdouble height = 0.7;
    const GLint slices = 20;
    const GLint stacks = 2;
    kvs::OpenGL::Translate( t );
    kvs::OpenGL::DrawCylinder( base, top, height, slices, stacks );
}

}; // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ArrowGlyph class.
 */
/*===========================================================================*/
ArrowGlyph::ArrowGlyph():
    kvs::GlyphBase(),
    m_type( ArrowGlyph::LineArrow ),
    m_volume( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ArrowGlyph class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
ArrowGlyph::ArrowGlyph( const kvs::VolumeObjectBase* volume ):
    kvs::GlyphBase(),
    m_type( ArrowGlyph::LineArrow ),
    m_volume( NULL )
{
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ArrowGlyph class.
 *  @param  volume [in] pointer to the Volume object
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
ArrowGlyph::ArrowGlyph(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::GlyphBase(),
    m_type( ArrowGlyph::LineArrow )
{
    BaseClass::setTransferFunction( transfer_function );
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] pointer to the volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void ArrowGlyph::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume ) { kvsMessageError("Input object is not volume dat."); return; }
    if ( m_volume != volume ) { this->attach_volume( volume ); }

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    this->initialize();
    this->draw();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Attaches a volume object.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void ArrowGlyph::attach_volume( const kvs::VolumeObjectBase* volume )
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
 *  @brief  Draw the arrow glyph.
 */
/*===========================================================================*/
void ArrowGlyph::draw()
{
    switch ( m_type )
    {
    case LineArrow: this->draw_lines(); break;
    case TubeArrow: this->draw_tubes(); break;
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the arrow glyph as lines.
 */
/*===========================================================================*/
void ArrowGlyph::draw_lines()
{
    const size_t npoints = BaseClass::coords().size() / 3;
    const kvs::ValueArray<kvs::Real32> coords = BaseClass::coords();
    const kvs::ValueArray<kvs::UInt8> colors = BaseClass::colors();
    const kvs::ValueArray<kvs::Real32> sizes = BaseClass::sizes();
    const kvs::ValueArray<kvs::UInt8> opacities = BaseClass::opacities();

    if ( BaseClass::directions().size() == 0 )
    {
        for ( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( coords.data() + index );
            const kvs::Real32 size = sizes[i];
            const kvs::RGBColor color( colors.data() + index );
            const kvs::UInt8 opacity = opacities[i];
            kvs::OpenGL::PushMatrix();
            {
                BaseClass::transform( position, size );
                this->draw_line_element( color, opacity );
            }
            kvs::OpenGL::PopMatrix();
        }
    }
    else
    {
        for( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( coords.data() + index );
            const kvs::Vector3f direction( BaseClass::directions().data() + index );
            const kvs::Real32 size = sizes[i];
            const kvs::RGBColor color( colors.data() + index );
            const kvs::UInt8 opacity = opacities[i];
            if ( direction.length() > 0.0f )
            {
                kvs::OpenGL::PushMatrix();
                {
                    BaseClass::transform( position, direction, size );
                    this->draw_line_element( color, opacity );
                }
                kvs::OpenGL::PopMatrix();
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the arrow glyph as polygons.
 */
/*===========================================================================*/
void ArrowGlyph::draw_tubes()
{
    const size_t npoints = BaseClass::coords().size() / 3;
    const kvs::ValueArray<kvs::Real32> coords = BaseClass::coords();
    const kvs::ValueArray<kvs::UInt8> colors = BaseClass::colors();
    const kvs::ValueArray<kvs::Real32> sizes = BaseClass::sizes();
    const kvs::ValueArray<kvs::UInt8> opacities = BaseClass::opacities();

    if ( BaseClass::directions().size() == 0 )
    {
        for ( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( coords.data() + index );
            const kvs::Real32 size = sizes[i];
            const kvs::RGBColor color( colors.data() + index );
            const kvs::UInt8 opacity = opacities[i];
            kvs::OpenGL::PushMatrix();
            {
                BaseClass::transform( position, size );
                this->draw_tube_element( color, opacity );
            }
            kvs::OpenGL::PopMatrix();
        }
    }
    else
    {
        for( size_t i = 0, index = 0; i < npoints; i++, index += 3 )
        {
            const kvs::Vector3f position( coords.data() + index );
            const kvs::Vector3f direction( BaseClass::directions().data() + index );
            const kvs::Real32 size = sizes[i];
            const kvs::RGBColor color( colors.data() + index );
            const kvs::UInt8 opacity = opacities[i];
            if ( direction.length() > 0.0f )
            {
                kvs::OpenGL::PushMatrix();
                {
                    BaseClass::transform( position, direction, size );
                    this->draw_tube_element( color, opacity );
                }
                kvs::OpenGL::PopMatrix();
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the line element.
 *  @param  color [in] color value
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void ArrowGlyph::draw_line_element( const kvs::RGBColor& color, const kvs::UInt8 opacity )
{
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
    for ( size_t i = 0; i < 6; i++ )
    {
        kvs::OpenGL::Vertex3( ::LineVertices + ::LineConnections[i] * 3 );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draw the tube element.
 *  @param  color [in] color value
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void ArrowGlyph::draw_tube_element( const kvs::RGBColor& color, const kvs::UInt8 opacity )
{
    const kvs::Real32 R = -90.0f; // rotation angle
    const kvs::Vec3 V( 1.0f, 0.0f, 0.0f ); // rotation vector
    const kvs::Vec3 T0( 0.0f, 0.0f, 0.7f ); // translation vector (cone)
    const kvs::Vec3 T1( 0.0f, 0.0f, 0.0f ); // translation vector (cylinder)

    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::Rotate( R, V );
    {
        kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );

        // Cone.
        kvs::OpenGL::PushMatrix();
        ::DrawCone( T0 );
        kvs::OpenGL::PopMatrix();

        // Cylinder.
        kvs::OpenGL::PushMatrix();
        ::DrawCylinder( T1 );
        kvs::OpenGL::PopMatrix();
    }
    kvs::OpenGL::PopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Initialize OpenGL properties for rendering arrow glyph.
 */
/*===========================================================================*/
void ArrowGlyph::initialize()
{
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::SetLightModel( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

    kvs::OpenGL::Disable( GL_LINE_SMOOTH );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );
    if ( m_type == ArrowGlyph::LineArrow )
    {
        kvs::OpenGL::Disable( GL_NORMALIZE );
        kvs::OpenGL::Disable( GL_LIGHTING );
    }
    else
    {
        if ( !BaseClass::isEnabledShading() )
        {
            kvs::OpenGL::Disable( GL_NORMALIZE );
            kvs::OpenGL::Disable( GL_LIGHTING );
        }
        else
        {
            kvs::OpenGL::Enable( GL_NORMALIZE );
            kvs::OpenGL::Enable( GL_LIGHTING );
        }
    }
}

} // end of namespace kvs
