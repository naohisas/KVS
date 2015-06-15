/*****************************************************************************/
/**
 *  @file   SphereGlyph.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SphereGlyph.cpp 1808 2014-08-30 00:00:29Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "SphereGlyph.h"
#include <kvs/OpenGL>


namespace
{

const int CacheSize = 240;
const float Pi = 3.14159265358979323846;

void DrawSphere( GLdouble radius, GLint slices, GLint stacks )
{
    GLint i,j;
    GLfloat sinCache1a[CacheSize];
    GLfloat cosCache1a[CacheSize];
    GLfloat sinCache2a[CacheSize];
    GLfloat cosCache2a[CacheSize];
    GLfloat sinCache3a[CacheSize];
    GLfloat cosCache3a[CacheSize];
    GLfloat sinCache1b[CacheSize];
    GLfloat cosCache1b[CacheSize];
    GLfloat sinCache2b[CacheSize];
    GLfloat cosCache2b[CacheSize];
    GLfloat sinCache3b[CacheSize];
    GLfloat cosCache3b[CacheSize];
    GLfloat angle;
    GLfloat zLow, zHigh;
    GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
    GLfloat costemp3 = 0.0, costemp4 = 0.0;
    GLboolean needCache2, needCache3;
    GLint start, finish;

    if (slices >= CacheSize) slices = CacheSize-1;
    if (stacks >= CacheSize) stacks = CacheSize-1;
    if (slices < 2 || stacks < 1 || radius < 0.0)
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = GL_TRUE;
    needCache3 = GL_FALSE;

    for (i = 0; i < slices; i++)
    {
        angle = 2 * Pi * i / slices;
        sinCache1a[i] = std::sin(angle);
        cosCache1a[i] = std::cos(angle);
        if (needCache2)
        {
            sinCache2a[i] = sinCache1a[i];
            cosCache2a[i] = cosCache1a[i];
        }
    }

    for (j = 0; j <= stacks; j++)
    {
        angle = Pi * j / stacks;
        if (needCache2)
        {
            sinCache2b[j] = std::sin(angle);
            cosCache2b[j] = std::cos(angle);
        }
        sinCache1b[j] = radius * std::sin(angle);
        cosCache1b[j] = radius * std::cos(angle);
    }
    /* Make sure it comes to a point */
    sinCache1b[0] = 0;
    sinCache1b[stacks] = 0;

    if (needCache3) {
        for (i = 0; i < slices; i++)
        {
            angle = 2 * Pi * (i-0.5) / slices;
            sinCache3a[i] = std::sin(angle);
            cosCache3a[i] = std::cos(angle);
        }
        for (j = 0; j <= stacks; j++)
        {
            angle = Pi * (j - 0.5) / stacks;
            sinCache3b[j] = std::sin(angle);
            cosCache3b[j] = std::cos(angle);
        }
    }

    sinCache1a[slices] = sinCache1a[0];
    cosCache1a[slices] = cosCache1a[0];
    if (needCache2)
    {
        sinCache2a[slices] = sinCache2a[0];
        cosCache2a[slices] = cosCache2a[0];
    }
    if (needCache3)
    {
        sinCache3a[slices] = sinCache3a[0];
        cosCache3a[slices] = cosCache3a[0];
    }

    /* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
    ** We don't do it when texturing because we need to respecify the
    ** texture coordinates of the apex for every adjacent vertex (because
    ** it isn't a constant for that point)
    */
    start = 1;
    finish = stacks - 1;

    /* Low end first (j == 0 iteration) */
    sintemp2 = sinCache1b[1];
    zHigh = cosCache1b[1];
    sintemp3 = sinCache2b[1];
    costemp3 = cosCache2b[1];
    KVS_GL_CALL_VER( glNormal3f(sinCache2a[0] * sinCache2b[0], cosCache2a[0] * sinCache2b[0], cosCache2b[0]) );

    KVS_GL_CALL_BEG( glBegin(GL_TRIANGLE_FAN) );
    KVS_GL_CALL_VER( glVertex3f(0.0, 0.0, radius) );
    for (i = slices; i >= 0; i--)
    {
        KVS_GL_CALL_VER( glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3) );
        KVS_GL_CALL_VER( glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh) );
    }
    KVS_GL_CALL_END( glEnd() );

    /* High end next (j == stacks-1 iteration) */
    sintemp2 = sinCache1b[stacks-1];
    zHigh = cosCache1b[stacks-1];
    sintemp3 = sinCache2b[stacks-1];
    costemp3 = cosCache2b[stacks-1];
    KVS_GL_CALL_VER( glNormal3f(sinCache2a[stacks] * sinCache2b[stacks], cosCache2a[stacks] * sinCache2b[stacks], cosCache2b[stacks]) );

    KVS_GL_CALL_BEG( glBegin(GL_TRIANGLE_FAN) );
    KVS_GL_CALL_VER( glVertex3f(0.0, 0.0, -radius) );
    for (i = 0; i <= slices; i++)
    {
        KVS_GL_CALL_VER( glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3) );
        KVS_GL_CALL_VER( glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh) );
    }
    KVS_GL_CALL_END( glEnd() );

    for (j = start; j < finish; j++)
    {
        zLow = cosCache1b[j];
        zHigh = cosCache1b[j+1];
        sintemp1 = sinCache1b[j];
        sintemp2 = sinCache1b[j+1];
        sintemp3 = sinCache2b[j+1];
        costemp3 = cosCache2b[j+1];
        sintemp4 = sinCache2b[j];
        costemp4 = cosCache2b[j];

        KVS_GL_CALL_BEG( glBegin(GL_QUAD_STRIP) );
        for (i = 0; i <= slices; i++)
        {
            KVS_GL_CALL_VER( glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3) );
            KVS_GL_CALL_VER( glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh) );

            KVS_GL_CALL_VER( glNormal3f(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4) );
            KVS_GL_CALL_VER( glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow) );
        }
        KVS_GL_CALL_END( glEnd() );
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new SphereGlyph class.
 */
/*===========================================================================*/
SphereGlyph::SphereGlyph():
    kvs::GlyphBase(),
    m_nslices( 10 ),
    m_nstacks( 10 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SphereGlyph class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
SphereGlyph::SphereGlyph( const kvs::PointObject* point ):
    kvs::GlyphBase(),
    m_nslices( 10 ),
    m_nstacks( 10 )
{
    this->attach_point( point );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SphereGlyph class.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
SphereGlyph::SphereGlyph( const kvs::VolumeObjectBase* volume ):
    kvs::GlyphBase(),
    m_nslices( 10 ),
    m_nstacks( 10 )
{
    this->attach_volume( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new SphereGlyph class.
 *  @param  volume [in] pointer to the volume object
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
SphereGlyph::SphereGlyph(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::GlyphBase(),
    m_nslices( 10 ),
    m_nstacks( 10 )
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
void SphereGlyph::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    kvs::IgnoreUnusedVariable( camera );

    if ( object->objectType() == kvs::ObjectBase::Geometry )
    {
        const kvs::GeometryObjectBase* geometry = kvs::GeometryObjectBase::DownCast( object );
        if ( geometry->geometryType() == kvs::GeometryObjectBase::Point )
        {
            const kvs::PointObject* point = kvs::PointObject::DownCast( geometry );
            if ( !point ) { kvsMessageError("Input object is not point dat."); return; }
            if ( m_point != point ) { this->attach_point( point ); }
        }
    }
    else if ( object->objectType() == kvs::ObjectBase::Volume )
    {
        const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
        if ( !volume ) { kvsMessageError("Input object is not volume dat."); return; }
        if ( m_volume != volume ) { this->attach_volume( volume ); }
    }

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    this->initialize();
    this->draw();
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Draw the sphere glyph.
 */
/*===========================================================================*/
void SphereGlyph::draw()
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
            kvs::OpenGL::PushMatrix();
            BaseClass::transform( position, size );
            this->draw_element( color, opacity );
            kvs::OpenGL::PopMatrix();
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
            kvs::OpenGL::PushMatrix();
            BaseClass::transform( position, direction, size );
            this->draw_element( color, opacity ); 
            kvs::OpenGL::PopMatrix();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Attaches a point object.
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
void SphereGlyph::attach_point( const kvs::PointObject* point )
{
    m_point = point;

    const size_t nvertices = point->numberOfVertices();

    BaseClass::setCoords( point->coords() );

    if ( BaseClass::directionMode() == BaseClass::DirectionByNormal )
    {
        if ( point->numberOfNormals() != 0 )
        {
            BaseClass::setDirections( point->normals() );
        }
    }

    if ( point->numberOfSizes() == 1 )
    {
        const kvs::Real32 size = point->size();
        kvs::ValueArray<kvs::Real32> sizes( nvertices );
        for ( size_t i = 0; i < nvertices; i++ ) sizes[i] = size;
        BaseClass::setSizes( sizes );
    }
    else
    {
        BaseClass::setSizes( point->sizes() );
    }

    if ( point->numberOfColors() == 1 )
    {
        const kvs::RGBColor color = point->color();
        kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
        for ( size_t i = 0, j = 0; i < nvertices; i++, j += 3 )
        {
            colors[j]   = color.r();
            colors[j+1] = color.g();
            colors[j+2] = color.b();
        }
        BaseClass::setColors( colors );
    }
    else
    {
        BaseClass::setColors( point->colors() );
    }

    const kvs::UInt8 opacity = static_cast<kvs::UInt8>( 255 );
    kvs::ValueArray<kvs::UInt8> opacities( nvertices );
    for ( size_t i = 0; i < nvertices; i++ ) opacities[i] = opacity;
    BaseClass::setOpacities( opacities );
}

/*===========================================================================*/
/**
 *  @brief  Attaches a volume object.
 *  @param  volume [in] pointer to the volume object
 *  @return glyph object
 */
/*===========================================================================*/
void SphereGlyph::attach_volume( const kvs::VolumeObjectBase* volume )
{
    m_volume = volume;

    BaseClass::calculateCoords( volume );

    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::Int8 ) )
    {
        BaseClass::calculateSizes<kvs::Int8>( volume );
        BaseClass::calculateColors<kvs::Int8>( volume );
        BaseClass::calculateOpacities<kvs::Int8>( volume );
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        BaseClass::calculateSizes<kvs::Int16>( volume );
        BaseClass::calculateColors<kvs::Int16>( volume );
        BaseClass::calculateOpacities<kvs::Int16>( volume );
    }
    else if ( type == typeid( kvs::Int32 ) )
    {
        BaseClass::calculateSizes<kvs::Int32>( volume );
        BaseClass::calculateColors<kvs::Int32>( volume );
        BaseClass::calculateOpacities<kvs::Int32>( volume );
    }
    else if ( type == typeid( kvs::Int64 ) )
    {
        BaseClass::calculateSizes<kvs::Int64>( volume );
        BaseClass::calculateColors<kvs::Int64>( volume );
        BaseClass::calculateOpacities<kvs::Int64>( volume );
    }
    else if ( type == typeid( kvs::UInt8  ) )
    {
        BaseClass::calculateSizes<kvs::UInt8>( volume );
        BaseClass::calculateColors<kvs::UInt8>( volume );
        BaseClass::calculateOpacities<kvs::UInt8>( volume );
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        BaseClass::calculateSizes<kvs::UInt16>( volume );
        BaseClass::calculateColors<kvs::UInt16>( volume );
        BaseClass::calculateOpacities<kvs::UInt16>( volume );
    }
    else if ( type == typeid( kvs::UInt32 ) )
    {
        BaseClass::calculateSizes<kvs::UInt32>( volume );
        BaseClass::calculateColors<kvs::UInt32>( volume );
        BaseClass::calculateOpacities<kvs::UInt32>( volume );
    }
    else if ( type == typeid( kvs::UInt64 ) )
    {
        BaseClass::calculateSizes<kvs::UInt64>( volume );
        BaseClass::calculateColors<kvs::UInt64>( volume );
        BaseClass::calculateOpacities<kvs::UInt64>( volume );
    }
    else if ( type == typeid( kvs::Real32 ) )
    {
        BaseClass::calculateSizes<kvs::Real32>( volume );
        BaseClass::calculateColors<kvs::Real32>( volume );
        BaseClass::calculateOpacities<kvs::Real32>( volume );
    }
    else if ( type == typeid( kvs::Real64 ) )
    {
        BaseClass::calculateSizes<kvs::Real64>( volume );
        BaseClass::calculateColors<kvs::Real64>( volume );
        BaseClass::calculateOpacities<kvs::Real64>( volume );
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the glyph element.
 *  @param  color [in] color value
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void SphereGlyph::draw_element( const kvs::RGBColor& color, const kvs::UInt8 opacity )
{
    KVS_GL_CALL_VER( glColor4ub( color.r(), color.g(), color.b(), opacity ) );

    const GLdouble radius = 0.5;
    ::DrawSphere( radius, static_cast<GLint>(m_nslices), static_cast<GLint>(m_nstacks) );
}

/*===========================================================================*/
/**
 *  @brief  Initialize the modelview matrix.
 */
/*===========================================================================*/
void SphereGlyph::initialize()
{
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

    kvs::OpenGL::Disable( GL_LINE_SMOOTH );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );
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

} // end of namespace kvs
