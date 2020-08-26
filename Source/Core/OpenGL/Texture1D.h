/****************************************************************************/
/**
 *  @file   Texture1D.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__TEXTURE_1D_H_INCLUDE
#define KVS__TEXTURE_1D_H_INCLUDE

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
public:

    typedef kvs::Texture BaseClass;

private:

    bool m_is_loaded; ///< if true, the texture is loaded

public:

    static void Unbind();

public:

    Texture1D();
    virtual ~Texture1D();

    bool isLoaded() const;

    void create( const size_t width, const void* data = NULL );
    void release();
    void load( const size_t width, const void* data, const size_t offset = 0 );
    void loadFromFrameBuffer( const int x, const int y, const size_t width, const size_t offset = 0 );

public:
    KVS_DEPRECATED( bool isDownloaded() const ) { return this->isLoaded(); }
    KVS_DEPRECATED( void download( const size_t width, const void* data, const size_t offset = 0 ) ) { this->load( width, data, offset ); }
};

} // end of namespace kvs

#endif // KVS__TEXTURE_1D_H_INCLUDE
