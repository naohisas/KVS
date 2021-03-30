/****************************************************************************/
/**
 *  @file   Texture1D.h
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
 *  1D texture class.
 */
/*==========================================================================*/
class Texture1D : public kvs::Texture
{
private:
    bool m_is_loaded = false; ///< if true, the texture is loaded

public:
    using BaseClass = kvs::Texture;
    static void Unbind();

public:
    Texture1D(): kvs::Texture( GL_TEXTURE_1D, GL_TEXTURE_BINDING_1D ) {}
    virtual ~Texture1D() { this->release(); }

    bool isLoaded() const { return m_is_loaded; }

    void create( const size_t width, const void* data = nullptr );
    void release();
    void load( const size_t width, const void* data, const size_t offset = 0 );
    void loadFromFrameBuffer( const int x, const int y, const size_t width, const size_t offset = 0 );
};

} // end of namespace kvs
