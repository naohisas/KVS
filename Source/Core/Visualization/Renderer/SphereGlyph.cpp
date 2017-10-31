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
#include <kvs/IgnoreUnusedVariable>


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
    const GLdouble radius = 0.5;
    const GLint slices = static_cast<GLint>( m_nslices );
    const GLint stacks = static_cast<GLint>( m_nstacks );

    kvs::OpenGL::Color( color.r(), color.g(), color.b(), opacity );
    kvs::OpenGL::DrawSphere( radius, slices, stacks );
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
