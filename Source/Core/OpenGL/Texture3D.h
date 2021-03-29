/****************************************************************************/
/**
 *  @file   Texture3D.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/Texture>
#include <kvs/Deprecated>
#include <cstddef>


namespace kvs
{

/*==========================================================================*/
/**
 *  3D texture class.
 */
/*==========================================================================*/
class Texture3D : public kvs::Texture
{
private:
    bool m_is_loaded = false; ///< if true, the texture is loaded

public:
    using BaseClass = kvs::Texture;
    static void Unbind();

public:
    Texture3D(): kvs::Texture( GL_TEXTURE_3D, GL_TEXTURE_BINDING_3D ) {}
    virtual ~Texture3D() { this->release(); }

    bool isLoaded() const { return m_is_loaded; }

    void create( const size_t width, const size_t height, const size_t depth, const void* data = NULL );
    void release();
    void load(
        const size_t width,
        const size_t height,
        const size_t depth,
        const void* data,
        const size_t xoffset = 0,
        const size_t yoffset = 0,
        const size_t zoffset = 0 );
    void loadFromFrameBuffer(
        const int x,
        const int y,
        const size_t width,
        const size_t height,
        const size_t depth,
        const size_t xoffset = 0,
        const size_t yoffset = 0,
        const size_t zoffset = 0 );

public:
    KVS_DEPRECATED( bool isDownloaded() const ) { return this->isLoaded(); }
    KVS_DEPRECATED( void download(
                        const size_t width,
                        const size_t height,
                        const size_t depth,
                        const void* data,
                        const size_t xoffset = 0,
                        const size_t yoffset = 0,
                        const size_t zoffset = 0 ) ) { return this->load( width, height, depth, data, xoffset, yoffset, zoffset ); }
};

} // end of namespace kvs
