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

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Background::Background()
{
    m_type = Background::MonoColor;
}

/*==========================================================================*/
/**
 *  Constructor for mono-color background.
 *  @param color [in] background color
 */
/*==========================================================================*/
Background::Background( const kvs::RGBAColor& color )
{
    this->setColor( color );
}

/*==========================================================================*/
/**
 *  Constructor for gradation background.
 *  @param color0 [in] color on the corner #0 (bottom side color)
 *  @param color1 [in] color on the corner #1 (top side color)
 */
/*==========================================================================*/
Background::Background( const kvs::RGBAColor& color1, const kvs::RGBAColor& color2 )
{
    this->setColor( color1, color2 );
}

/*==========================================================================*/
/**
 *  Constructor for gradation background.
 *  @param color0 [in] color on the corner #0 (bottom-left color)
 *  @param color1 [in] color on the corner #1 (bottom-right color)
 *  @param color2 [in] color on the corner #2 (top-right color)
 *  @param color3 [in] color on the corner #3 (top-left color)
 */
/*==========================================================================*/
Background::Background(
    const kvs::RGBAColor& color0,
    const kvs::RGBAColor& color1,
    const kvs::RGBAColor& color2,
    const kvs::RGBAColor& color3 )
{
    this->setColor( color0, color1, color2, color3 );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Background::~Background()
{
}

/*==========================================================================*/
/**
 *  Copy operator.
 *  @param bg [in] background object
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
 *  Set the color.
 *  @param color [in] color
 */
/*==========================================================================*/
void Background::setColor( const kvs::RGBAColor& color )
{
    m_type = Background::MonoColor;
    m_color[0] = color;
}

/*==========================================================================*/
/**
 *  Set the colors.
 *  @param color0 [in] color on the corner #0 (bottom side color)
 *  @param color1 [in] color on the corner #1 (top side color)
 */
/*==========================================================================*/
void Background::setColor( const RGBAColor& color0, const RGBAColor& color1 )
{
    m_type = Background::TwoSideColor;
    m_color[0] = color0;
    m_color[1] = color0;
    m_color[2] = color1;
    m_color[3] = color1;
}

/*==========================================================================*/
/**
 *  Set the colors.
 *  @param color0 [in] color on the corner #0 (bottom-left color)
 *  @param color1 [in] color on the corner #1 (bottom-right color)
 *  @param color2 [in] color on the corner #2 (top-right color)
 *  @param color3 [in] color on the corner #3 (top-left color)
 */
/*==========================================================================*/
void Background::setColor(
    const kvs::RGBAColor& color0,
    const kvs::RGBAColor& color1,
    const kvs::RGBAColor& color2,
    const kvs::RGBAColor& color3 )
{
    m_type = Background::FourCornersColor;
    m_color[0] = color0;
    m_color[1] = color1;
    m_color[2] = color2;
    m_color[3] = color3;
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
 *  Get the background color.
 *  @param index [in] color index (0-3)
 */
/*==========================================================================*/
const kvs::RGBColor& Background::color( size_t index ) const
{
    return m_color[index];
}

/*==========================================================================*/
/**
 *  Apply gradation color or image to the background.
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
 *  Apply mono color to the background.
 */
/*==========================================================================*/
void Background::apply_mono_color()
{
    float r = static_cast<float>( m_color[0].r() ) / 255.0f;
    float g = static_cast<float>( m_color[0].g() ) / 255.0f;
    float b = static_cast<float>( m_color[0].b() ) / 255.0f;
    KVS_GL_CALL( glClearDepth( 1.0 ) );
    KVS_GL_CALL( glClearColor( r, g, b, 1.0f ) );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*==========================================================================*/
/**
 *  Apply gradation color to the background.
 */
/*==========================================================================*/
void Background::apply_gradation_color()
{
    KVS_GL_CALL( glClearDepth( 1.0 ) );
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
                kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
                glBegin( GL_QUADS );
                glColor3ub( m_color[0].r(), m_color[0].g(), m_color[0].b() ); glVertex2d( 0.0, 0.0 );
                glColor3ub( m_color[1].r(), m_color[1].g(), m_color[1].b() ); glVertex2d( 1.0, 0.0 );
                glColor3ub( m_color[2].r(), m_color[2].g(), m_color[2].b() ); glVertex2d( 1.0, 1.0 );
                glColor3ub( m_color[3].r(), m_color[3].g(), m_color[3].b() ); glVertex2d( 0.0, 1.0 );
                glEnd();
            }
        }
    }
}

/*==========================================================================*/
/**
 *  Apply image to the background.
 */
/*==========================================================================*/
void Background::apply_image()
{
    KVS_GL_CALL( glClearDepth( 1.0 ) );
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
                kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
                glBegin( GL_QUADS );
                glTexCoord2f( 0.0, 0.0 ); glVertex2f(  1.0,  1.0 );
                glTexCoord2f( 0.0, 1.0 ); glVertex2f(  1.0,  0.0 );
                glTexCoord2f( 1.0, 1.0 ); glVertex2f(  0.0,  0.0 );
                glTexCoord2f( 1.0, 0.0 ); glVertex2f(  0.0,  1.0 );
                glEnd();
            }
        }
    }
}

} // end of namespace kvs

