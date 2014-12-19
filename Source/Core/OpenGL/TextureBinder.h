/*****************************************************************************/
/**
 *  @file   TextureBinder.h
 *  @author Naoya Maeda, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__TEXTURE_BINDER_H_INCLUDE
#define KVS__TEXTURE_BINDER_H_INCLUDE

#include <kvs/GL>


namespace kvs
{

class Texture;

class TextureBinder
{
private:

    const kvs::Texture& m_texture;
    GLint m_unit;

public:

    TextureBinder( const kvs::Texture& texture, const GLint unit = 0 );
    ~TextureBinder();

private:

    TextureBinder( const TextureBinder& );
    TextureBinder& operator =( const TextureBinder& );
};

} // end of namespace kvs

#endif // KVS__TEXTURE_BINDER_H_INCLUDE
