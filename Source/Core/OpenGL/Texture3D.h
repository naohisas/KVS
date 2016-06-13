/****************************************************************************/
/**
 *  @file   Texture3D.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture3D.h 1573 2013-05-17 09:49:35Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__TEXTURE_3D_H_INCLUDE
#define KVS__TEXTURE_3D_H_INCLUDE

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
public:

    typedef kvs::Texture BaseClass;

private:

    bool m_is_loaded; ///< if true, the texture is loaded

public:

    static void Unbind();

public:

    Texture3D();
    virtual ~Texture3D();

    bool isLoaded() const;

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

#endif // KVS__TEXTURE_3D_H_INCLUDE
