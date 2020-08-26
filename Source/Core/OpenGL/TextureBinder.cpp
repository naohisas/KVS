/*****************************************************************************/
/**
 *  @file   TextureBinder.cpp
 *  @author Naoya Maeda, Naohisa Sakamoto
 */
/*****************************************************************************/
#include "TextureBinder.h"
#include <kvs/OpenGL>
#include <kvs/Assert>
#include <kvs/Texture>


namespace kvs
{

TextureBinder::TextureBinder( const Texture& texture, GLint unit ) :
    m_texture( texture ),
    m_unit( unit )
{
    KVS_ASSERT( texture.isCreated() );
    kvs::OpenGL::ActivateTextureUnit( unit );
    texture.bind();
}

TextureBinder::~TextureBinder()
{
    KVS_ASSERT( m_texture.isCreated() );
    kvs::OpenGL::ActivateTextureUnit( m_unit );
    KVS_GL_CALL( glBindTexture( m_texture.target(), 0 ) );
}

} // end of namespace kvs
