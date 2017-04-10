/*****************************************************************************/
/**
 *  @file   TextEngine.cpp
 *  @author Naohisa Sakamoto
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
#include "TextEngine.h"
#include <kvs/OpenGL>


namespace kvs
{

TextEngine::TextEngine()
{
}

TextEngine::TextEngine( const kvs::Font& font ):
    m_font( font )
{
}

TextEngine::TextEngine( TextEngine* engine ):
    m_font( engine->m_font )
{
}

TextEngine::~TextEngine()
{
}

int TextEngine::width( const char c ) const
{
    return this->width( std::string( 1, c ) );
}

int TextEngine::width( const std::string& text ) const
{
    return m_font.width( text );
}

int TextEngine::height() const
{
    return m_font.height();
}

void TextEngine::draw( const kvs::Vec2i& p, const std::string& text, kvs::ScreenBase* screen ) const
{
    this->draw( kvs::Vec2( p ), text, screen );
}

void TextEngine::draw( const kvs::Vec2& p, const std::string& text, kvs::ScreenBase* screen ) const
{
    GLint view[4]; kvs::OpenGL::GetViewport( view );

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
//    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.disable( GL_DEPTH_TEST );
    attrib.enable( GL_BLEND );
    attrib.enable( GL_CULL_FACE );
    {
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        kvs::OpenGL::WithPushedMatrix modelview( GL_MODELVIEW );
        modelview.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix projection( GL_PROJECTION );
            projection.loadIdentity();
            {
                kvs::OpenGL::Enable( GL_TEXTURE_2D );
                kvs::OpenGL::Enable( GL_BLEND );
                kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

                const GLint left = view[0];
                const GLint top = view[1];
                const GLint right = view[0] + view[2];
                const GLint bottom = view[1] + view[3];
                kvs::OpenGL::SetOrtho( left, right, bottom, top, 0, 1 );
                m_font.draw( p, text );
            }
        }
    }
}

void TextEngine::draw( const kvs::Vec3& p, const std::string& text, kvs::ScreenBase* screen ) const
{
    GLdouble model[16]; kvs::OpenGL::GetModelViewMatrix( model );
    GLdouble proj[16]; kvs::OpenGL::GetProjectionMatrix( proj );
    GLint view[4]; kvs::OpenGL::GetViewport( view );
    GLdouble winx = 0, winy = 0, winz = 0;
    kvs::OpenGL::Project( p.x(), p.y(), p.z(), model, proj, view, &winx, &winy, &winz );

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
//    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.enable( GL_DEPTH_TEST );
    attrib.enable( GL_BLEND );
    {
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        kvs::OpenGL::WithPushedMatrix modelview( GL_MODELVIEW );
        modelview.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix projection( GL_PROJECTION );
            projection.loadIdentity();
            {
                const GLint left = view[0];
                const GLint top = view[1];
                const GLint right = view[0] + view[2];
                const GLint bottom = view[1] + view[3];
                kvs::OpenGL::SetOrtho( left, right, bottom, top, 0, 1 );
                kvs::OpenGL::Translate( 0, 0, -winz );
                m_font.draw( kvs::Vec2( winx, top - ( winy - bottom ) ), text );
            }
        }
    }
}

void TextEngine::draw( const kvs::Vec2& p, const kvs::Font::Icon& icon, const float size ) const
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
//    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.disable( GL_DEPTH_TEST );
    attrib.enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    m_font.draw( p, icon, size );
}

} // end of namespace kvs
