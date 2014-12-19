/****************************************************************************/
/**
 *  @file   ParticleBufferAccumulator.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBufferAccumulator.cpp 1644 2013-09-17 08:25:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ParticleBufferAccumulator.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Construct a new class.
 *  @param width [in] width
 *  @param height [in] height
 *  @param subpixel_level [in] subpixel level
 */
/*==========================================================================*/
ParticleBufferAccumulator::ParticleBufferAccumulator(
    const size_t width,
    const size_t height,
    const size_t subpixel_level ):
    kvs::ParticleBuffer( width, height, subpixel_level )
{
    this->create( width, height, subpixel_level );
}

/*==========================================================================*/
/**
 *  Destruct this class.
 */
/*==========================================================================*/
ParticleBufferAccumulator::~ParticleBufferAccumulator()
{
    this->clear();
}

/*==========================================================================*/
/**
 *  Create a new class.
 *  @param width [in] width
 *  @param height [in] height
 *  @param subpixel_level [in] subpixel level
 *  @return true/false
 */
/*==========================================================================*/
bool ParticleBufferAccumulator::create(
    const size_t width,
    const size_t height,
    const size_t subpixel_level )
{
    const size_t npixels = width * height;
    const size_t subpixeled_npixels = npixels * subpixel_level * subpixel_level;

    m_id_buffer.allocate( subpixeled_npixels );
    m_id_buffer.fill( 0 );

    return( true );
}

/*==========================================================================*/
/**
 *  Clean this class. (not delete)
 */
/*==========================================================================*/
void ParticleBufferAccumulator::clean()
{
    SuperClass::clean();
    m_id_buffer.fill( 0 );
}

/*==========================================================================*/
/**
 *  Clear this class. (delete this class)
 */
/*==========================================================================*/
void ParticleBufferAccumulator::clear()
{
    SuperClass::clear();
    m_id_buffer.release();
}

/*==========================================================================*/
/**
 *  Accumulate the particle buffer.
 *  @param id [in] ID number of the object
 *  @param buffer [in] pointer to the particle buffer
 */
/*==========================================================================*/
void ParticleBufferAccumulator::accumulate(
    const size_t id,
    const kvs::ParticleBuffer* buffer )
{
    const size_t nsubpixels = m_width * m_height * m_subpixel_level * m_subpixel_level;
    for( size_t index = 0; index < nsubpixels; index++ )
    {
        const kvs::Real32 buffer_depth = buffer->depth( index );
        if( buffer_depth > 0.0f )
        {
            const kvs::UInt32 buffer_index = buffer->index( index );
            this->add( index, id, buffer_depth, buffer_index );
        }
    }
}

/*==========================================================================*/
/**
 *  Add a particle to the buffer for accumulating.
 *  @param index [in] subpixel index
 *  @param id [in] ID number of the object
 *  @param depth [in] buffer depth
 *  @param vindex [in] buffer index
 */
/*==========================================================================*/
void ParticleBufferAccumulator::add(
    const size_t index,
    const size_t id,
    const kvs::Real32 depth,
    const kvs::UInt32 vindex )
{
    const kvs::Real32 buffer_depth = SuperClass::depth( index );
    if ( buffer_depth > 0.0f )
    {
        // Detect collision.
        if ( buffer_depth > depth )
        {
            SuperClass::depthBuffer()[index] = depth;
            SuperClass::indexBuffer()[index] = vindex;
            m_id_buffer[index] = static_cast<kvs::UInt8>( id );
        }
    }
    else
    {
        // Not collision.
        SuperClass::depthBuffer()[index] = depth;
        SuperClass::indexBuffer()[index] = vindex;
        m_id_buffer[index] = static_cast<kvs::UInt8>( id );
    }
}

/*==========================================================================*/
/**
 *  Create color and depth buffer.
 *  @param object_list [in] object list
 *  @param renderer_list [in] renderer list
 *  @param color [in] color buffer
 *  @param depth [in] depth buffer
 */
/*==========================================================================*/
void ParticleBufferAccumulator::createImage(
    ParticleBufferAccumulator::ObjectList&   object_list,
    ParticleBufferAccumulator::RendererList& renderer_list,
    kvs::ValueArray<kvs::UInt8>*  color,
    kvs::ValueArray<kvs::Real32>* depth )
{
    const float inv_ssize = 1.0f / ( m_subpixel_level * m_subpixel_level );
    const float normalize_alpha = 255.0f * inv_ssize;

    size_t pindex   = 0;
    size_t pindex4  = 0;
    size_t by_start = 0;
    const size_t bw = m_width * m_subpixel_level;
    for ( size_t py = 0; py < m_height; py++, by_start += m_subpixel_level )
    {
        size_t bx_start = 0;
        for( size_t px = 0; px < m_width; px++, pindex++, pindex4 += 4, bx_start += m_subpixel_level )
        {
            float R = 0.0f;
            float G = 0.0f;
            float B = 0.0f;
            float D = 0.0f;
            size_t npoints = 0;
            for( size_t by = by_start; by < by_start + m_subpixel_level; by++ )
            {
                const size_t bindex_start = bw * by;
                for( size_t bx = bx_start; bx < bx_start + m_subpixel_level; bx++ )
                {
                    const size_t bindex = bindex_start + bx;
                    if( m_depth_buffer[bindex] > 0.0f )
                    {
                        const size_t id = m_id_buffer[ bindex ];
                        const kvs::PointObject*            object   = object_list[id];
                        const kvs::ParticleVolumeRenderer* renderer = renderer_list[id];

                        const size_t point_index3 = 3 * m_index_buffer[bindex];

                        kvs::RGBColor color( object->colors().data() + point_index3 );
                        if( renderer->isEnabledShading() )
                        {
                            const kvs::Shader::ShadingModel* shader = renderer->particleBuffer()->shader();
                            const kvs::Vector3f vertex( object->coords().data() + point_index3 );
                            const kvs::Vector3f normal( object->normals().data() + point_index3 );
                            color = shader->shadedColor( color, vertex, normal );
                        }

                        R += color.r();
                        G += color.g();
                        B += color.b();
                        D = kvs::Math::Max( D, m_depth_buffer[ bindex ] );

                        npoints++;
                    }
                }
            }

            R *= inv_ssize;
            G *= inv_ssize;
            B *= inv_ssize;

            (*color)[ pindex4 + 0 ] = static_cast<kvs::UInt8>(R);
            (*color)[ pindex4 + 1 ] = static_cast<kvs::UInt8>(G);
            (*color)[ pindex4 + 2 ] = static_cast<kvs::UInt8>(B);
            (*color)[ pindex4 + 3 ] = static_cast<kvs::UInt8>( npoints * normalize_alpha );
            (*depth)[ pindex ]      = ( npoints == 0 ) ? 1.0f : D;
        }
    }
}

} // end of namespace kvs
