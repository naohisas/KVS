/****************************************************************************/
/**
 *  @file   ParticleBuffer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBuffer.h 1643 2013-09-17 08:23:01Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Shader>
#include <kvs/Deprecated>


namespace kvs
{

class PointObject;

/*==========================================================================*/
/**
 *  Particle buffer class.
 */
/*==========================================================================*/
class ParticleBuffer
{
protected:
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    size_t m_size; ///< pixel data size [byte]
    size_t m_num_of_projected_particles; ///< total number of projected points
    size_t m_num_of_stored_particles; ///< total number of stored points
    size_t m_subpixel_level; ///< subpixel level
    bool m_enable_shading; ///< shading flag
    size_t m_extended_width; ///< m_width * m_subpixel_level
    size_t m_device_pixel_ratio; ///< device pixel ratio
    kvs::ValueArray<kvs::UInt32> m_index_buffer; ///< index buffer
    kvs::ValueArray<kvs::Real32> m_depth_buffer; ///< depth buffer

    // Reference shader (NOTE: not allocated in thie class).
    const kvs::Shader::ShadingModel* m_ref_shader;
    const kvs::PointObject* m_ref_point_object;

public:
    ParticleBuffer();
    ParticleBuffer( const size_t width, const size_t height, const size_t subpixel_level, const size_t device_pixel_ratio = 1.0f );
    virtual ~ParticleBuffer();

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    const kvs::ValueArray<kvs::UInt32>& indexBuffer() const { return m_index_buffer; }
    kvs::UInt32 index( const size_t index ) const { return m_index_buffer[index]; }
    const kvs::ValueArray<kvs::Real32>& depthBuffer() const { return m_depth_buffer; }
    kvs::Real32 depth( const size_t index ) const { return m_depth_buffer[index]; }
    size_t subpixelLevel() const { return m_subpixel_level; }
    const kvs::Shader::ShadingModel* shader() const { return m_ref_shader; }
    const kvs::PointObject* pointObject() const { return m_ref_point_object; }
    size_t numberOfProjectedParticles() const { return m_num_of_projected_particles; }
    size_t numberOfStoredParticles() const { return m_num_of_stored_particles; }
    void setSubpixelLevel( const size_t subpixel_level ) { m_subpixel_level = subpixel_level; }
    void attachShader( const kvs::Shader::ShadingModel* shader ) { m_ref_shader = shader; }
    void attachPointObject( const kvs::PointObject* point_object ) { m_ref_point_object = point_object; }
    void enableShading() { m_enable_shading = true; }
    void disableShading() { m_enable_shading = false; }

    void add( const float x, const float y, const kvs::Real32 depth, const kvs::UInt32 index );
    bool create( const size_t width, const size_t height, const size_t subpixel_level, const size_t device_pixel_ratio = 1.0f );
    void clean();
    void clear();
    void createImage( kvs::ValueArray<kvs::UInt8>* color, kvs::ValueArray<kvs::Real32>* depth );

protected:
    kvs::ValueArray<kvs::UInt32>& indexBuffer() { return m_index_buffer; }
    kvs::ValueArray<kvs::Real32>& depthBuffer() { return m_depth_buffer; }

private:
    void create_image_with_shading( kvs::ValueArray<kvs::UInt8>* color, kvs::ValueArray<kvs::Real32>* depth );
    void create_image_without_shading( kvs::ValueArray<kvs::UInt8>* color, kvs::ValueArray<kvs::Real32>* depth );

public:
    KVS_DEPRECATED( size_t numOfProjectedParticles() const ) { return this->numberOfProjectedParticles(); }
    KVS_DEPRECATED( size_t numOfStoredParticles() const ) { return this->numberOfStoredParticles(); }
};

/*==========================================================================*/
/**
 *  Add a point to the buffer.
 *  @param x [in] x coordinate value in the buffer
 *  @param y [in] y coordinate value in the buffer
 *  @param depth [in] depth value
 *  @param voxel_index [in] voxel index
 */
/*==========================================================================*/
inline void ParticleBuffer::add(
    const float x,
    const float y,
    const kvs::Real32 depth,
    const kvs::UInt32 voxel_index )
{
    // Buffer coordinate value.
    const size_t bx = static_cast<size_t>( x * m_subpixel_level );
    const size_t by = static_cast<size_t>( y * m_subpixel_level );

    const size_t index = m_extended_width * by + bx;
    m_num_of_projected_particles++;

    if( m_depth_buffer[index] > 0.0f )
    {
        // Detect collision.
        if( m_depth_buffer[index] > depth )
        {
            m_depth_buffer[index] = depth;
            m_index_buffer[index] = voxel_index;
        }
    }
    else
    {
        m_depth_buffer[index] = depth;
        m_index_buffer[index] = voxel_index;
    }
}

} // end of namespace kvs
