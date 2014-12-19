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

const kvs::Vector3f ConeTranslation = kvs::Vector3f( 0.0f, 0.0f, 0.7f );
const kvs::Real32 ConeHeight = 0.3f;
const kvs::Real32 ConeRadius = 0.15f;
const size_t ConeSlices = 20;
const size_t ConeStacks = 5;

const kvs::Vector3f CylinderTranslation = kvs::Vector3f( 0.0f, 0.0f, 0.0f );
const kvs::Real32 CylinderHeight = 0.7f;
const kvs::Real32 CylinderRadius = 0.07f;
const size_t CylinderSlices = 20;
const size_t CylinderStacks = 2;

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
    m_cylinder( NULL ),
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
    m_cylinder( NULL ),
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
    m_cylinder( NULL ),
    m_type( ArrowGlyph::LineArrow )
{
    BaseClass::setTransferFunction( transfer_function );
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Arrow glyph class.
 */
/*===========================================================================*/
ArrowGlyph::~ArrowGlyph()
{
    gluDeleteQuadric( m_cylinder );
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
    if ( !m_cylinder )
    {
        m_cylinder = gluNewQuadric();
        gluQuadricDrawStyle( m_cylinder, GLU_FILL );
        gluQuadricNormals( m_cylinder, GLU_SMOOTH );
    }

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
            glPushMatrix();
            {
                BaseClass::transform( position, size );
                this->draw_line_element( color, opacity );
            }
            glPopMatrix();
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
                glPushMatrix();
                {
                    BaseClass::transform( position, direction, size );
                    this->draw_line_element( color, opacity );
                }
                glPopMatrix();
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
            glPushMatrix();
            {
                BaseClass::transform( position, size );
                this->draw_tube_element( color, opacity );
            }
            glPopMatrix();
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
                glPushMatrix();
                {
                    BaseClass::transform( position, direction, size );
                    this->draw_tube_element( color, opacity );
                }
                glPopMatrix();
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
    glColor4ub( color.r(), color.g(), color.b(), opacity );

    glBegin( GL_LINES );
    for ( size_t i = 0; i < 6; i++ )
    {
        const kvs::Real32* vertex = ::LineVertices + ::LineConnections[i] * 3;
        glVertex3fv( vertex );
    }
    glEnd();
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
    glColor4ub( color.r(), color.g(), color.b(), opacity );

    glPushMatrix();
    glRotated( -90.0, 1.0, 0.0, 0.0 );

    // Cone.
    glPushMatrix();
    glTranslatef( ::ConeTranslation.x(), ::ConeTranslation.y(), ::ConeTranslation.z() );
    gluCylinder( m_cylinder, ::ConeRadius, 0.0, ::ConeHeight, ::ConeSlices, ::ConeStacks );
    glPopMatrix();

    // Cylinder.
    glPushMatrix();
    glTranslatef( ::CylinderTranslation.x(), ::CylinderTranslation.y(), ::CylinderTranslation.z() );
    gluCylinder( m_cylinder, ::CylinderRadius, ::CylinderRadius, ::CylinderHeight, ::CylinderSlices, ::CylinderStacks );
    glPopMatrix();

    glPopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Initialize OpenGL properties for rendering arrow glyph.
 */
/*===========================================================================*/
void ArrowGlyph::initialize()
{
    glDisable( GL_LINE_SMOOTH );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glShadeModel( GL_SMOOTH );

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    if ( m_type == ArrowGlyph::LineArrow )
    {
        glDisable( GL_NORMALIZE );
        glDisable( GL_LIGHTING );
    }
    else
    {
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

    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
}

} // end of namespace kvs
