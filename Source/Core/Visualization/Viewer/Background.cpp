/****************************************************************************/
/**
 *  @file   Background.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Background.cpp 1587 2013-06-05 02:50:50Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Background.h"
#include <kvs/OpenGL>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Background class.
 */
/*===========================================================================*/
Background::Background()
{
    m_type = Background::MonoColor;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Background class.
 *  @param  c [in] background color
 */
/*==========================================================================*/
Background::Background( const kvs::RGBAColor& c )
{
    this->setColor( c );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Background class.
 *  @param  c0 [in] color on the corner #0 (bottom side color)
 *  @param  c1 [in] color on the corner #1 (top side color)
 */
/*==========================================================================*/
Background::Background( const kvs::RGBAColor& c0, const kvs::RGBAColor& c1 )
{
    this->setColor( c0, c1 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Background class.
 *  @param  c0 [in] color on the corner #0 (bottom-left color)
 *  @param  c1 [in] color on the corner #1 (bottom-right color)
 *  @param  c2 [in] color on the corner #2 (top-right color)
 *  @param  c3 [in] color on the corner #3 (top-left color)
 */
/*==========================================================================*/
Background::Background(
    const kvs::RGBAColor& c0,
    const kvs::RGBAColor& c1,
    const kvs::RGBAColor& c2,
    const kvs::RGBAColor& c3 )
{
    this->setColor( c0, c1, c2, c3 );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Background class.
 */
/*===========================================================================*/
Background::~Background()
{
}

/*==========================================================================*/
/**
 *  @brief  Copies a backbround to this backbround.
 *  @param  bg [in] background object
 */
/*==========================================================================*/
Background& Background::operator = ( const Background& bg )
{
    m_type = bg.m_type;
    m_color[0] = bg.m_color[0];
    m_color[1] = bg.m_color[1];
    m_color[2] = bg.m_color[2];
    m_color[3] = bg.m_color[3];
    return *this;
}

/*==========================================================================*/
/**
 *  @brief  Sets the color.
 *  @param  c [in] color
 */
/*==========================================================================*/
void Background::setColor( const kvs::RGBAColor& c )
{
    m_type = Background::MonoColor;
    m_color[0] = c;
}

/*==========================================================================*/
/**
 *  @brief  Sets the colors.
 *  @param  c0 [in] color on the corner #0 (bottom side color)
 *  @param  c1 [in] color on the corner #1 (top side color)
 */
/*==========================================================================*/
void Background::setColor( const RGBAColor& c0, const RGBAColor& c1 )
{
    m_type = Background::TwoSideColor;
    m_color[0] = c0;
    m_color[1] = c0;
    m_color[2] = c1;
    m_color[3] = c1;
}

/*==========================================================================*/
/**
 *  @brief  Set the colors.
 *  @param  c0 [in] color on the corner #0 (bottom-left color)
 *  @param  c1 [in] color on the corner #1 (bottom-right color)
 *  @param  c2 [in] color on the corner #2 (top-right color)
 *  @param  c3 [in] color on the corner #3 (top-left color)
 */
/*==========================================================================*/
void Background::setColor(
    const kvs::RGBAColor& c0,
    const kvs::RGBAColor& c1,
    const kvs::RGBAColor& c2,
    const kvs::RGBAColor& c3 )
{
    m_type = Background::FourCornersColor;
    m_color[0] = c0;
    m_color[1] = c1;
    m_color[2] = c2;
    m_color[3] = c3;
}

/*===========================================================================*/
/**
 *  @brief  Sets an image data.
 *  @param  image [in] input data
 */
/*===========================================================================*/
void Background::setImage( const kvs::ColorImage& image )
{
    m_type = Background::Image;
    m_image = image;
    m_image_changed = true;
}

/*==========================================================================*/
/**
 *  @brief  Returns the background color.
 *  @param  index [in] color index (0-3)
 */
/*==========================================================================*/
const kvs::RGBColor& Background::color( size_t index ) const
{
    return m_color[index];
}

/*==========================================================================*/
/**
 *  @brief  Applies gradation color or image to the background.
 */
/*==========================================================================*/
void Background::apply()
{
    switch ( m_type )
    {
    case Background::MonoColor: this->apply_mono_color(); break;
    case Background::TwoSideColor: this->apply_gradation_color(); break;
    case Background::FourCornersColor: this->apply_gradation_color(); break;
    case Background::Image: this->apply_image(); break;
    default: break;
    }
    kvs::OpenGL::Flush();
}

/*==========================================================================*/
/**
 *  @brief  Applies the mono color to the background.
 */
/*==========================================================================*/
void Background::apply_mono_color()
{
    kvs::OpenGL::SetClearDepth( 1.0 );
    kvs::OpenGL::SetClearColor( m_color[0] );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*==========================================================================*/
/**
 *  @brief  Applies the gradation colors to the background.
 */
/*==========================================================================*/
void Background::apply_gradation_color()
{
    kvs::OpenGL::SetClearDepth( 1.0f );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    kvs::OpenGL::WithPushedAttrib a( GL_ALL_ATTRIB_BITS );
    {
        kvs::OpenGL::Disable( GL_BLEND );
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Disable( GL_LIGHTING );
        kvs::OpenGL::Disable( GL_TEXTURE_1D );
        kvs::OpenGL::Disable( GL_TEXTURE_2D );
        kvs::OpenGL::Disable( GL_TEXTURE_3D );

        kvs::OpenGL::WithPushedMatrix m( GL_MODELVIEW );
        m.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix p( GL_PROJECTION );
            p.loadIdentity();
            {
                kvs::OpenGL::SetOrtho( 0, 1, 0, 1, 0, 1 );
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::Color( m_color[0] ); kvs::OpenGL::Vertex( kvs::Vec2( 0, 0 ) );
                kvs::OpenGL::Color( m_color[1] ); kvs::OpenGL::Vertex( kvs::Vec2( 1, 0 ) );
                kvs::OpenGL::Color( m_color[2] ); kvs::OpenGL::Vertex( kvs::Vec2( 1, 1 ) );
                kvs::OpenGL::Color( m_color[3] ); kvs::OpenGL::Vertex( kvs::Vec2( 0, 1 ) );
                kvs::OpenGL::End();
            }
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Applies the image to the background.
 */
/*==========================================================================*/
void Background::apply_image()
{
    kvs::OpenGL::SetClearDepth( 1.0 );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    kvs::OpenGL::WithPushedAttrib a( GL_ALL_ATTRIB_BITS );
    {
        kvs::OpenGL::Disable( GL_BLEND );
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Disable( GL_LIGHTING );
        kvs::OpenGL::Disable( GL_TEXTURE_1D );
        kvs::OpenGL::Disable( GL_TEXTURE_3D );
        kvs::OpenGL::Enable( GL_TEXTURE_2D );

        if ( m_image_changed )
        {
            m_texture.release();
            m_texture.setWrapS( GL_CLAMP_TO_EDGE );
            m_texture.setWrapT( GL_CLAMP_TO_EDGE );
            m_texture.setMagFilter( GL_LINEAR );
            m_texture.setMinFilter( GL_LINEAR );
            m_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
            m_texture.create( m_image.width(), m_image.height(), m_image.pixels().data() );
            m_image_changed = false;
        }

        kvs::Texture::Binder binder( m_texture );
        kvs::OpenGL::WithPushedMatrix m( GL_MODELVIEW );
        m.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix p( GL_PROJECTION );
            p.loadIdentity();
            {
                kvs::OpenGL::SetOrtho( 0, 1, 0, 1, 0, 1 );
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 0 ), kvs::Vec2( 1, 1 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 1 ), kvs::Vec2( 1, 0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 1 ), kvs::Vec2( 0, 0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 0 ), kvs::Vec2( 0, 1 ) );
                kvs::OpenGL::End();
            }
        }
    }
}

} // end of namespace kvs
