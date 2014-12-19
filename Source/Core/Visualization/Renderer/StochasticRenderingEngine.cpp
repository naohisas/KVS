/*****************************************************************************/
/**
 *  @file   StochasticRenderingEngine.cpp
 *  @author Jun Nishimura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
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

} // end of namespace kvs
