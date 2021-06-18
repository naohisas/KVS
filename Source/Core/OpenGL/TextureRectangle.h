/****************************************************************************/
/**
 *  @file   TextureRectangle.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/Texture>
#include <kvs/Deprecated>
#include <cstddef>

#ifndef GL_TEXTURE_RECTANGLE
#define GL_TEXTURE_RECTANGLE 0x84F5
#endif

#ifndef GL_TEXTURE_BINDING_RECTANGLE
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Texture rectangle class.
 */
/*==========================================================================*/
class TextureRectangle : public kvs::Texture
{
private:
    bool m_is_loaded = false; ///< if true, the texture is loaded

public:
    using BaseClass = kvs::Texture;
    static void Unbind();

public:
    TextureRectangle(): kvs::Texture( GL_TEXTURE_RECTANGLE, GL_TEXTURE_BINDING_RECTANGLE ) {}
    virtual ~TextureRectangle() { this->release(); }

    bool isLoaded() const { return m_is_loaded; }

    void create( const size_t width, const size_t height, const void* data = NULL );
    void release();
    void load(
        const size_t width,
        const size_t height,
        const void*  data,
        const size_t xoffset = 0,
        const size_t yoffset = 0 );
};

} // end of namespace kvs
