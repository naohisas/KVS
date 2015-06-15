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


namespace
{

const int CacheSize = 240;
const float Pi = 3.14159265358979323846;

void DrawCylinder(
    GLdouble baseRadius,
    GLdouble topRadius,
    GLdouble height,
    GLint slices,
    GLint stacks )
{
    GLint i,j;
    GLfloat sinCache[CacheSize];
    GLfloat cosCache[CacheSize];
    GLfloat sinCache2[CacheSize];
    GLfloat cosCache2[CacheSize];
    GLfloat sinCache3[CacheSize];
    GLfloat cosCache3[CacheSize];
    GLfloat angle;
    GLfloat zLow, zHigh;
    GLfloat length;
    GLfloat deltaRadius;
    GLfloat zNormal;
    GLfloat xyNormalRatio;
    GLfloat radiusLow, radiusHigh;
    int needCache2, needCache3;

    if (slices >= CacheSize) slices = CacheSize-1;

    if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 || height < 0.0)
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Compute length (needed for normal calculations) */
    deltaRadius = baseRadius - topRadius;
    length = std::sqrt(deltaRadius*deltaRadius + height*height);
    if ( length == 0.0 )
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = 1;
    needCache3 = 0;

    zNormal = deltaRadius / length;
    xyNormalRatio = height / length;

    for (i = 0; i < slices; i++)
    {
        angle = 2 * Pi * i / slices;
        if (needCache2)
        {
            sinCache2[i] = xyNormalRatio * std::sin(angle);
            cosCache2[i] = xyNormalRatio * std::cos(angle);
        }
        sinCache[i] = std::sin(angle);
        cosCache[i] = std::cos(angle);
    }

    if (needCache3)
    {
        for (i = 0; i < slices; i++)
        {
            angle = 2 * Pi * (i-0.5) / slices;
            sinCache3[i] = xyNormalRatio * std::sin(angle);
            cosCache3[i] = xyNormalRatio * std::cos(angle);
        }
    }

    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    if (needCache2)
    {
        sinCache2[slices] = sinCache2[0];
        cosCache2[slices] = cosCache2[0];
    }
    if (needCache3)
    {
        sinCache3[slices] = sinCache3[0];
        cosCache3[slices] = cosCache3[0];
    }

    /* Note:
    ** An argument could be made for using a TRIANGLE_FAN for the end
    ** of the cylinder of either radii is 0.0 (a cone).  However, a
    ** TRIANGLE_FAN would not work in smooth shading mode (the common
    ** case) because the normal for the apex is different for every
    ** triangle (and TRIANGLE_FAN doesn't let me respecify that normal).
    ** Now, my choice is GL_TRIANGLES, or leave the GL_QUAD_STRIP and
    ** just let the GL trivially reject one of the two triangles of the
    ** QUAD.  GL_QUAD_STRIP is probably faster, so I will leave this code
    ** alone.
    */
    for (j = 0; j < stacks; j++)
    {
        zLow = j * height / stacks;
        zHigh = (j + 1) * height / stacks;
        radiusLow = baseRadius - deltaRadius * ((float) j / stacks);
        radiusHigh = baseRadius - deltaRadius * ((float) (j + 1) / stacks);

        KVS_GL_CALL_BEG( glBegin(GL_QUAD_STRIP) );
        for (i = 0; i <= slices; i++)
        {
            KVS_GL_CALL_VER( glNormal3f(sinCache2[i], cosCache2[i], zNormal) );
            KVS_GL_CALL_VER( glVertex3f(radiusLow  * sinCache[i], radiusLow  * cosCache[i], zLow) );
            KVS_GL_CALL_VER( glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh) );
        }
        KVS_GL_CALL_END( glEnd() );
    }
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
    KVS_GL_CALL_BEG( glBegin( GL_LINES ) );
    KVS_GL_CALL_VER( glColor4ub( color.r(), color.g(), color.b(), opacity ) );
    for ( size_t i = 0; i < 6; i++ )
    {
        const kvs::Real32* vertex = ::LineVertices + ::LineConnections[i] * 3;
        KVS_GL_CALL_VER( glVertex3fv( vertex ) );
    }
    KVS_GL_CALL_END( glEnd() );
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
    KVS_GL_CALL_VER( glColor4ub( color.r(), color.g(), color.b(), opacity ) );

    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::Rotate( -90.0f, 1.0f, 0.0f, 0.0f );

    // Cone.
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::Translate( ::ConeTranslation.x(), ::ConeTranslation.y(), ::ConeTranslation.z() );
    ::DrawCylinder( ::ConeRadius, 0.0, ::ConeHeight, ::ConeSlices, ::ConeStacks );
    kvs::OpenGL::PopMatrix();

    // Cylinder.
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::Translate( ::CylinderTranslation.x(), ::CylinderTranslation.y(), ::CylinderTranslation.z() );
    ::DrawCylinder( ::CylinderRadius, ::CylinderRadius, ::CylinderHeight, ::CylinderSlices, ::CylinderStacks );
    kvs::OpenGL::PopMatrix();

    kvs::OpenGL::PopMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Initialize OpenGL properties for rendering arrow glyph.
 */
/*===========================================================================*/
void ArrowGlyph::initialize()
{
    kvs::OpenGL::Disable( GL_LINE_SMOOTH );

    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    kvs::OpenGL::SetShadeModel( GL_SMOOTH );

    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
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

    KVS_GL_CALL( glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE ) );
}

} // end of namespace kvs
