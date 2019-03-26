/****************************************************************************/
/**
 *  @file LegendBar.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LegendBar.cpp 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "LegendBar.h"
#include <kvs/Type>
#include <kvs/Message>
#include <kvs/String>
#include <kvs/Math>
#include <kvs/EventBase>
#include <kvs/IgnoreUnusedVariable>


// Default parameters.
namespace { namespace Default
{
const double MinValue = 0.0f;
const double MaxValue = 255.0f;
const size_t LegendBarWidth = 200;
const size_t LegendBarHeight = 20;
const size_t LegendBarMargin = 10;
} }


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new LegendBar class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
LegendBar::LegendBar( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_show_range_value( true ),
    m_texture_downloaded( false )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( ::Default::LegendBarMargin );
    this->setCaption( "" );
    this->setOrientation( LegendBar::Horizontal );
    this->setNumberOfDivisions( 5 );
    this->setDivisionLineWidth( 1.0f );
    this->setDivisionLineColor( kvs::RGBColor( 0, 0, 0 ) );
    this->setRange( ::Default::MinValue, ::Default::MaxValue );
    this->setBorderWidth( 1.0f );
    this->setBorderColor( kvs::RGBColor( 0, 0, 0 ) );
    this->disableAntiAliasing();

    m_colormap.setResolution( 256 );
    m_colormap.create();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the LegendBar class.
 */
/*===========================================================================*/
LegendBar::~LegendBar()
{
}

/*===========================================================================*/
/**
 *  @brief  Set the color map to the texture.
 *  @param  colormap [in] color map
 */
/*===========================================================================*/
void LegendBar::setColorMap( const kvs::ColorMap& colormap )
{
    // Deep copy.
    kvs::ColorMap::Table colormap_table( colormap.table().data(), colormap.table().size() );
    m_colormap = kvs::ColorMap( colormap_table );

    if ( colormap.hasRange() )
    {
        m_min_value = colormap.minValue();
        m_max_value = colormap.maxValue();
    }

    // Download the texture data onto GPU.
    m_texture_downloaded = false;
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void LegendBar::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    if ( !m_texture_downloaded )
    {
         this->create_texture();
        m_texture_downloaded = true;
    }

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    const std::string min_value = kvs::String::ToString( m_min_value );
    const std::string max_value = kvs::String::ToString( m_max_value );
    const int character_width  = BaseClass::characterWidth();
    const int character_height = BaseClass::characterHeight();
    const int caption_height = ( m_caption.size() == 0 ) ? 0 : character_height + 5;
    const int value_width = character_width * kvs::Math::Max( min_value.size(), max_value.size() );
    const int value_height = ( m_show_range_value ) ? character_height : 0;

    // Draw the color bar.
    {
        const int w = ( m_orientation == LegendBar::Vertical ) ? value_width + 5 : 0;
        const int h = ( m_orientation == LegendBar::Vertical ) ? 0 : value_height;
        const int x = BaseClass::x0() + m_margin;
        const int y = BaseClass::y0() + m_margin + caption_height;
        const int width = BaseClass::width() - m_margin * 2 - w;
        const int height = BaseClass::height() - m_margin * 2 - caption_height - h;
        this->draw_color_bar( x, y, width, height );
        this->draw_border( x, y, width, height );
    }

    // Draw the caption.
    if ( m_caption.size() != 0 )
    {
        const int x = BaseClass::x0() + m_margin;
        const int y = BaseClass::y0() + m_margin;
        BaseClass::drawText( x, y + character_height, m_caption );
    }

    // Draw the values.
    if ( m_show_range_value )
    {
        switch ( m_orientation )
        {
        case LegendBar::Horizontal:
        {
            {
                const int x = BaseClass::x0() + m_margin;
                const int y = BaseClass::y1() - m_margin - character_height;
                BaseClass::drawText( x, y + character_height, min_value );
            }
            {
                const int x = BaseClass::x1() - m_margin - max_value.size() * character_width;
                const int y = BaseClass::y1() - m_margin - character_height;
                BaseClass::drawText( x, y + character_height, max_value );
            }
            break;
        }
        case LegendBar::Vertical:
        {
            {
                const int x = BaseClass::x1() - m_margin - value_width;
                const int y = BaseClass::y0() + m_margin + caption_height;
                BaseClass::drawText( x, y + character_height, min_value );
            }
            {
                const int x = BaseClass::x1() - m_margin - value_width;
                const int y = BaseClass::y1() - m_margin - character_height;
                BaseClass::drawText( x, y + character_height, max_value );
            }
            break;
        }
        default: break;
        }
    }

    BaseClass::render2D().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void LegendBar::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );
    this->screenResized();
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen width.
 *  @return screen width
 */
/*===========================================================================*/
int LegendBar::adjustedWidth()
{
    size_t width = 0;
    switch ( m_orientation )
    {
    case LegendBar::Horizontal:
    {
        width = m_caption.size() * BaseClass::characterWidth() + BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::Default::LegendBarWidth );
        break;
    }
    case LegendBar::Vertical:
    {
        std::string min_value = kvs::String::ToString( m_min_value );
        std::string max_value = kvs::String::ToString( m_max_value );
        width = BaseClass::characterWidth() * kvs::Math::Max( m_caption.size(), min_value.size(), max_value.size() );
        width += BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::Default::LegendBarHeight );
        break;
    }
    default: break;
    }

    return( static_cast<int>( width ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen height.
 *  @return screen height
 */
/*===========================================================================*/
int LegendBar::adjustedHeight()
{
    size_t height = 0;
    switch( m_orientation )
    {
    case LegendBar::Horizontal:
        height = ::Default::LegendBarHeight + ( BaseClass::characterHeight() + BaseClass::margin() ) * 2;
        break;
    case LegendBar::Vertical:
        height = ::Default::LegendBarWidth + BaseClass::characterHeight() + BaseClass::margin() * 2;
        break;
    default: break;
    }

    return( static_cast<int>( height ) );
}

/*===========================================================================*/
/**
 *  @brief  Create a texture for the color map.
 */
/*===========================================================================*/
void LegendBar::create_texture()
{
    const size_t      nchannels  = 3;
    const size_t      width  = m_colormap.resolution();
    const size_t      height = 1;
    const kvs::UInt8* data   = m_colormap.table().data();

    m_texture.release();
    m_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
    m_texture.setMinFilter( GL_NEAREST );
    m_texture.setMagFilter( GL_NEAREST );
    m_texture.create( width, height, data );
}

/*===========================================================================*/
/**
 *  @brief  Release the texture for the color map.
 */
/*===========================================================================*/
void LegendBar::release_texture()
{
    m_texture.release();
}

/*===========================================================================*/
/**
 *  @brief  Draws the color bar.
 *  @param  x [in] x position of the bar
 *  @param  y [in] y position of the bar
 *  @param  width [in] bar width
 *  @param  height [in] bar height
 */
/*===========================================================================*/
void LegendBar::draw_color_bar( const int x, const int y, const int width, const int height )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_BLEND );
    attrib.disable( GL_DEPTH_TEST );
    attrib.disable( GL_TEXTURE_3D );
    attrib.enable( GL_TEXTURE_2D );

    kvs::Texture::Binder binder( m_texture );
    switch ( m_orientation )
    {
    case LegendBar::Horizontal:
    {
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x,         y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x + width, y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x + width, y + height ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x,         y + height ) );
        kvs::OpenGL::End();
        break;
    }
    case LegendBar::Vertical:
    {
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x,         y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x + width, y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x + width, y + height ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x,         y + height ) );
        kvs::OpenGL::End();
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draws the border of the color map.
 *  @param  x [in] x position of the color map
 *  @param  y [in] y position of the color map
 *  @param  width [in] width
 *  @param  height [in] height
 */
/*===========================================================================*/
void LegendBar::draw_border( const int x, const int y, const int width, const int height )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );

    if ( m_enable_anti_aliasing && m_border_width != 1.0f )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    kvs::OpenGL::SetLineWidth( m_border_width );
    kvs::OpenGL::Begin( GL_LINE_LOOP );
    kvs::OpenGL::Color( m_border_color );
    kvs::OpenGL::Vertices( kvs::Vec2( x, y ), kvs::Vec2( x + width, y ), kvs::Vec2( x + width, y + height ), kvs::Vec2( x, y + height ) );
    kvs::OpenGL::End();

    if ( m_enable_anti_aliasing && m_border_width != 1.0f )
    {
        kvs::OpenGL::Disable( GL_LINE_SMOOTH );
        kvs::OpenGL::Disable( GL_BLEND );
    }

    if ( m_ndivisions == 0 ) return;

    if ( m_enable_anti_aliasing && m_division_line_width != 1.0f )
    {
        kvs::OpenGL::Enable( GL_LINE_SMOOTH );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    kvs::OpenGL::SetLineWidth( m_division_line_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_division_line_color );
    switch ( m_orientation )
    {
    case LegendBar::Horizontal:
    {
        const float w = width / m_ndivisions;
        const float h = height;
        for( size_t i = 1; i < m_ndivisions; i++ )
        {
            kvs::OpenGL::Vertices( kvs::Vec2( x + w * i, y ), kvs::Vec2( x + w * i, y + h ) );
        }
        break;
    }
    case LegendBar::Vertical:
    {
        const float w = width;
        const float h = height / m_ndivisions;
        for( size_t i = 1; i < m_ndivisions; i++ )
        {
            kvs::OpenGL::Vertices( kvs::Vec2( x, y + h * i ), kvs::Vec2( x + w, y + h * i ) );
        }
        break;
    }
    default: break;
    }
    kvs::OpenGL::End();
}

} // end of namespace glut

} // end of namespace kvs
