/****************************************************************************/
/**
 *  @file   Texture2D.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture2D.cpp 1555 2013-04-21 02:31:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Texture2D.h"
#include <kvs/Math>
#include <kvs/OpenGL>
#include <kvs/Assert>
#include <iostream>


namespace kvs
{

void Texture2D::Unbind()
{
    kvs::Texture::Unbind( GL_TEXTURE_2D );
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Texture2D::Texture2D():
    Texture( GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D ),
    m_is_loaded( false )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Texture2D::~Texture2D()
{
    this->release();
}

bool Texture2D::isLoaded() const
{
    return m_is_loaded;
}

/*==========================================================================*/
/**
 *  Create the texture.
 */
/*==========================================================================*/
void Texture2D::create( const size_t width, const size_t height, const void* data )
{
    KVS_ASSERT( width > 0 );
    KVS_ASSERT( static_cast<GLint>( width ) <= kvs::OpenGL::MaxTextureSize() );
    KVS_ASSERT( height > 0 );
    KVS_ASSERT( static_cast<GLint>( height ) <= kvs::OpenGL::MaxTextureSize() );

    BaseClass::createID();
    BaseClass::setSize( width, height );
    BaseClass::GuardedBinder binder( *this );
    BaseClass::setParameter( GL_TEXTURE_MAG_FILTER, BaseClass::magFilter() );
    BaseClass::setParameter( GL_TEXTURE_MIN_FILTER, BaseClass::minFilter() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_S, BaseClass::wrapS() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_T, BaseClass::wrapT() );
    this->load( width, height, data );
}

/*==========================================================================*/
/**
 *  Release the texture.
 */
/*==========================================================================*/
void Texture2D::release()
{
    BaseClass::deleteID();
    m_is_loaded = false;
}

/*==========================================================================*/
/**
 *  Download the texture data to the GPU.
 *  @param width [in] texture width
 *  @param height [in] texture height
 *  @param data [in] pointer to the pixel data
 *  @param xoffset [in] texel offset in the x direction within the pixel data
 *  @param yoffset [in] texel offset in the y direction within the pixel data
 */
/*==========================================================================*/
void Texture2D::load(
    const size_t width,
    const size_t height,
    const void* data,
    const size_t xoffset,
    const size_t yoffset )
{
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::setImage2D( width, height, data );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::setSubImage2D( width, height, data, xoffset, yoffset );
    }

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

void Texture2D::loadFromFrameBuffer(
    const int x,
    const int y,
    const size_t width,
    const size_t height,
    const size_t xoffset,
    const size_t yoffset )
{
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::copyImage2D( x, y, width, height );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::copySubImage2D( x, y, width, height, xoffset, yoffset );
    }

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

} // end of namespace kvs
