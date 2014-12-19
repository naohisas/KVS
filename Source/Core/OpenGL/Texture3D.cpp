/****************************************************************************/
/**
 *  @file   Texture3D.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture3D.cpp 1555 2013-04-21 02:31:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Texture3D.h"
#include <kvs/Math>
#include <kvs/OpenGL>
#include <kvs/Assert>
#include <iostream>


namespace kvs
{

void Texture3D::Unbind()
{
    kvs::Texture::Unbind( GL_TEXTURE_3D );
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Texture3D::Texture3D():
    Texture( GL_TEXTURE_3D, GL_TEXTURE_BINDING_3D ),
    m_is_loaded( false )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Texture3D::~Texture3D()
{
    this->release();
}

bool Texture3D::isLoaded() const
{
    return m_is_loaded;
}

/*==========================================================================*/
/**
 *  Create the texture.
 */
/*==========================================================================*/
void Texture3D::create( const size_t width, const size_t height, const size_t depth, const void* data )
{
    KVS_ASSERT( width > 0 );
    KVS_ASSERT( static_cast<GLint>( width ) <= kvs::OpenGL::MaxTextureSize() );
    KVS_ASSERT( height > 0 );
    KVS_ASSERT( static_cast<GLint>( height ) <= kvs::OpenGL::MaxTextureSize() );
    KVS_ASSERT( depth > 0 );
    KVS_ASSERT( static_cast<GLint>( depth ) <= kvs::OpenGL::MaxTextureSize() );

    BaseClass::createID();
    BaseClass::setSize( width, height, depth );
    BaseClass::GuardedBinder binder( *this );
    BaseClass::setParameter( GL_TEXTURE_MAG_FILTER, BaseClass::magFilter() );
    BaseClass::setParameter( GL_TEXTURE_MIN_FILTER, BaseClass::minFilter() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_S, BaseClass::wrapS() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_T, BaseClass::wrapT() );
    BaseClass::setParameter( GL_TEXTURE_WRAP_R, BaseClass::wrapR() );
    this->load( width, height, depth, data );
}

/*==========================================================================*/
/**
 *  Release the texture.
 */
/*==========================================================================*/
void Texture3D::release()
{
    BaseClass::deleteID();
    m_is_loaded = false;
}

/*==========================================================================*/
/**
 *  Load the texture data to the GPU.
 *  @param width  [in] texture width
 *  @param height [in] texture height
 *  @param depth  [in] texture depth
 *  @param data [in] pointer to the pixel data
 *  @param xoffset [in] texel offset in the x direction within the pixel data
 *  @param yoffset [in] texel offset in the y direction within the pixel data
 *  @param zoffset [in] texel offset in the z direction within the pixel data
 */
/*==========================================================================*/
void Texture3D::load(
    const size_t width,
    const size_t height,
    const size_t depth,
    const void*  data,
    const size_t xoffset,
    const size_t yoffset,
    const size_t zoffset )
{
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    if ( !m_is_loaded )
    {
        BaseClass::setImage3D( width, height, depth, data );
        m_is_loaded = true;
    }
    else
    {
        BaseClass::setSubImage3D( width, height, depth, data, xoffset, yoffset, zoffset );
    }

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

void Texture3D::loadFromFrameBuffer(
    const int x,
    const int y,
    const size_t width,
    const size_t height,
    const size_t depth,
    const size_t xoffset,
    const size_t yoffset,
    const size_t zoffset )
{
    KVS_ASSERT( this->isLoaded() );
    const GLint swap = kvs::OpenGL::Integer( GL_UNPACK_SWAP_BYTES );
    const GLint alignment = kvs::OpenGL::Integer( GL_UNPACK_ALIGNMENT );
    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap ? GL_TRUE : GL_FALSE );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, 1 );

    BaseClass::copySubImage3D( x, y, width, height, xoffset, yoffset, zoffset );

    BaseClass::setPixelStorageMode( GL_UNPACK_SWAP_BYTES, swap );
    BaseClass::setPixelStorageMode( GL_UNPACK_ALIGNMENT, alignment );
}

} // end of namespace kvs
