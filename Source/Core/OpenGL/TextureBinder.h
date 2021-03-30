/*****************************************************************************/
/**
 *  @file   TextureBinder.h
 *  @author Naoya Maeda, Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    TextureBinder( const TextureBinder& ) = delete;
    TextureBinder& operator =( const TextureBinder& ) = delete;
};

} // end of namespace kvs
