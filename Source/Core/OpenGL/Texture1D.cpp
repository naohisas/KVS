/****************************************************************************/
/**
 *  @file   Texture1D.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Texture1D.h"
#include <kvs/Math>
#include <kvs/OpenGL>
#include <kvs/Assert>
#include <iostream>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Unbinds 1D textures.
 */
/*===========================================================================*/
void Texture1D::Unbind()
{
    kvs::Texture::Unbind( GL_TEXTURE_1D );
}

/*==========================================================================*/
/**
 *  @brief  Create the texture.
 */
/*==========================================================================*/
void Texture1D::create( const size_t width, const void* data )
{
    KVS_ASSERT( width > 0 );
    KVS_ASSERT( static_cast<GLint>( width ) <= kvs::OpenGL::MaxTextureSize() );

    BaseClass::createID();
    BaseClass::setSize( width );
    BaseClass::GuardedBinder binder( *this );
    BaseClass::setParameter( GL_TEXTURE_MAG_FILTER, BaseClass::magFilter() );
    BaseClass::setParameter( GL_TEXTURE_MIN_FILTER, BaseClass::minFilter() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_S, BaseClass::wrapS() );
    this->load( width, data );
}

/*==========================================================================*/
/**
 *  @brief  Release the texture.
 */
/*==========================================================================*/
void Texture1D::release()
{
    BaseClass::deleteID();
    m_is_loaded = false;
}

/*==========================================================================*/
/**
 *  @brief  Load texture data to the GPU.
 *  @param  width [in] texture width
 *  @param  data [in] pointer to the pixel data
 *  @param  offset [in] texel offset in the x direction within the pixel data
 */
/*==========================================================================*/
void Texture1D::load(
    const size_t width,
    const void* data,
    const size_t offset )
{
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::setImage1D( width, data );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::setSubImage1D( width, data, offset );
    }

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

/*==========================================================================*/
/**
 *  @brief  Load texture data to from frame buffer.
 *  @param  x [in] x position of the left corner of the row of pixels
 *  @param  y [in] y position of the left corner of the row of pixels
 *  @param  width [in] texture width
 *  @param  offset [in] texel offset in the x-direction within the pixel data
 */
/*==========================================================================*/
void Texture1D::loadFromFrameBuffer(
    const int x,
    const int y,
    const size_t width,
    const size_t offset )
{
//    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
//    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
//    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
//    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::copyImage1D( x, y, width );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::copySubImage1D( x, y, width, offset );
    }

//    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
//    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

} // end of namespace kvs
