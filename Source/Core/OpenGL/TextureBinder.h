/*****************************************************************************/
/**
 *  @file   TextureBinder.h
 *  @author Naoya Maeda, Naohisa Sakamoto
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
