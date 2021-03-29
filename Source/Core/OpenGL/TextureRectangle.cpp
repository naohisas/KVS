/****************************************************************************/
/**
 *  @file   TextureRectangle.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "TextureRectangle.h"
#include <kvs/Math>
#include <kvs/Assert>
#include <kvs/OpenGL>
#include <iostream>

//#ifndef GL_TEXTURE_RECTANGLE
//#define GL_TEXTURE_RECTANGLE 0x84F5
//#endif

//#ifndef GL_TEXTURE_BINDING_RECTANGLE
//#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
//#endif


namespace kvs
{

void TextureRectangle::Unbind()
{
    kvs::Texture::Unbind( GL_TEXTURE_RECTANGLE );
}

/*==========================================================================*/
/**
 *  Create the texture.
 */
/*==========================================================================*/
void TextureRectangle::create( const size_t width, const size_t height, const void* data )
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
void TextureRectangle::release()
{
    BaseClass::deleteID();
    m_is_loaded = false;
}

/*==========================================================================*/
/**
 *  Load the texture data to the GPU.
 *  @param width [in] texture width
 *  @param height [in] texture height
 *  @param pixels [in] pointer to the pixel data
 *  @param xoffset [in] texel offset in the x direction within the pixel data
 *  @param yoffset [in] texel offset in the y direction within the pixel data
 */
/*==========================================================================*/
void TextureRectangle::load(
    const size_t width,
    const size_t height,
    const void*  data,
    const size_t xoffset,
    const size_t yoffset )
{
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::setImageRectangle( width, height, data );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::setSubImageRectangle( width, height, data, xoffset, yoffset );
    }

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

} // end of namespace kvs
