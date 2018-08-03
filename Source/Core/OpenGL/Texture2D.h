/****************************************************************************/
/**
 *  @file   Texture2D.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Texture2D.h 1551 2013-04-20 01:40:09Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TEXTURE_2D_H_INCLUDE
#define KVS__TEXTURE_2D_H_INCLUDE

#include <kvs/Texture>
#include <kvs/Deprecated>
#include <cstddef>


namespace kvs
{

/*==========================================================================*/
/**
 *  2D texture class.
 */
/*==========================================================================*/
class Texture2D : public kvs::Texture
{
public:

    typedef kvs::Texture BaseClass;

private:

    bool m_is_loaded; ///< if true, the texture is loaded

public:

    static void Unbind();

public:

    Texture2D();
    virtual ~Texture2D();

    bool isLoaded() const;

    void create( const size_t width, const size_t height, const void* data = NULL );
    void release();
    void load(
        const size_t width,
        const size_t height,
        const void* data,
        const size_t xoffset = 0,
        const size_t yoffset = 0 );
    void loadFromFrameBuffer(
        const int x,
        const int y,
        const size_t width,
        const size_t height,
        const size_t xoffset = 0,
        const size_t yoffset = 0 );

public:
    KVS_DEPRECATED( bool isDownload() const ) { return this->isLoaded(); }
    KVS_DEPRECATED( void download(
                        const size_t width,
                        const size_t height,
                        const void* data,
                        const size_t xoffset = 0,
                        const size_t yoffset = 0 ) ) { this->load( width, height, data, xoffset, yoffset ); }
};

} // end of namespace kvs

#endif // KVS_CORE_TEXTURE_2D_H_INCLUDE
