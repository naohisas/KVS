/*****************************************************************************/
/**
 *  @file   StochasticRenderingEngine.cpp
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StochasticRenderingEngine.h"
#include <kvs/Assert>
#include <kvs/Xorshift128>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticRenderingEngine class.
 */
/*===========================================================================*/
StochasticRenderingEngine::StochasticRenderingEngine():
    m_object( NULL ),
    m_shader( NULL ),
    m_enable_shading( true ),
    m_repetition_level( 1 ),
    m_repetition_count( 0 ),
    m_random_texture_size( 512 )
{
}

/*===========================================================================*/
/**
 *  @brief  Creates random texture.
 */
/*===========================================================================*/
void StochasticRenderingEngine::createRandomTexture()
{
    KVS_ASSERT( m_random_texture_size > 0 );

    kvs::Xorshift128 R;
    kvs::ValueArray<kvs::Real32> random( m_random_texture_size * m_random_texture_size );
    for ( size_t i = 0; i < random.size(); i++ ) { random[i] = R.rand(); }

    m_random_texture.release();
    m_random_texture.setWrapS( GL_REPEAT );
    m_random_texture.setWrapT( GL_REPEAT );
    m_random_texture.setMagFilter( GL_NEAREST );
    m_random_texture.setMinFilter( GL_NEAREST );
    m_random_texture.setPixelFormat( GL_INTENSITY,  GL_LUMINANCE, GL_FLOAT );
    m_random_texture.create( m_random_texture_size, m_random_texture_size, random.data() );
}

kvs::ValueArray<kvs::UInt16> StochasticRenderingEngine::randomIndices( const size_t nvertices ) const
{
    const auto tex_size = this->randomTextureSize();
    kvs::ValueArray<kvs::UInt16> indices( nvertices * 2 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        const unsigned int count = i * 12347;
        indices[ 2 * i + 0 ] = static_cast<kvs::UInt16>( ( count ) % tex_size );
        indices[ 2 * i + 1 ] = static_cast<kvs::UInt16>( ( count / tex_size ) % tex_size );
    }
    return indices;
}

} // end of namespace kvs
