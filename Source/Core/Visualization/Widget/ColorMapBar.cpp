/*****************************************************************************/
/**
 *  @file   ColorMapBar.cpp
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
#include "ColorMapBar.h"
#include <kvs/Type>
#include <kvs/Message>
#include <kvs/String>
#include <kvs/Math>
#include <kvs/EventBase>
#include <kvs/IgnoreUnusedVariable>


// Constant variables
namespace
{
const double MinValue = 0.0f;
const double MaxValue = 255.0f;
const size_t ColorMapBarWidth = 150;
const size_t ColorMapBarHeight = 30;
const size_t ColorMapBarMargin = 10;
}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ColorMapBar class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
ColorMapBar::ColorMapBar( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_show_range_value( true ),
    m_texture_downloaded( false )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( ::ColorMapBarMargin );
    this->setCaption( "" );
    this->setOrientation( ColorMapBar::Horizontal );
    this->setNumberOfDivisions( 5 );
    this->setDivisionLineWidth( 1.0f );
    this->setDivisionLineColor( kvs::RGBColor( 0, 0, 0 ) );
    this->setRange( ::MinValue, ::MaxValue );
    this->setBorderWidth( 1.0f );
    this->setBorderColor( kvs::RGBColor( 0, 0, 0 ) );
    this->disableAntiAliasing();

    m_colormap.setResolution( 256 );
    m_colormap.create();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ColorMapBar class.
 */
/*===========================================================================*/
ColorMapBar::~ColorMapBar()
{
}

/*===========================================================================*/
/**
 *  @brief  Set the color map to the texture.
 *  @param  colormap [in] color map
 */
/*===========================================================================*/
void ColorMapBar::setColorMap( const kvs::ColorMap& colormap )
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
void ColorMapBar::paintEvent()
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
    const int text_height = BaseClass::textEngine().height();
    const int min_text_width = BaseClass::textEngine().width( min_value );
    const int max_text_width = BaseClass::textEngine().width( max_value );
    const int caption_height = ( m_caption.size() == 0 ) ? 0 : text_height + 5;
    const int value_width = ( min_value.size() > max_value.size() ) ? min_text_width : max_text_width;
    const int value_height = ( m_show_range_value ) ? text_height : 0;

    // Draw the color bar.
    {
        const int w = ( m_orientation == ColorMapBar::Vertical ) ? value_width + 5 : 0;
        const int h = ( m_orientation == ColorMapBar::Vertical ) ? 0 : value_height;
        const int x = m_x + BaseClass::margin();
        const int y = m_y + BaseClass::margin() + caption_height;
        const int width = m_width - BaseClass::margin() * 2 - w;
        const int height = m_height - BaseClass::margin() * 2 - caption_height - h;
        this->draw_color_bar( x, y, width, height );
        this->draw_border( x, y, width, height );
    }

    // Draw the caption.
    if ( m_caption.size() != 0 )
    {
        const int x = m_x + BaseClass::margin();
        const int y = m_y + BaseClass::margin();
        const kvs::Vec2 p( x, y + text_height );
        BaseClass::textEngine().draw( p, m_caption, BaseClass::screen() );
    }

    // Draw the values.
    if ( m_show_range_value )
    {
        switch ( m_orientation )
        {
        case ColorMapBar::Horizontal:
        {
            {
                const int x = m_x + BaseClass::margin();
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::textEngine().draw( p, min_value, BaseClass::screen() );
            }
            {
                const int x = BaseClass::x1() - BaseClass::margin() - max_text_width;
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::textEngine().draw( p, max_value, BaseClass::screen() );
            }
            break;
        }
        case ColorMapBar::Vertical:
        {
            {
                const int x = BaseClass::x1() - BaseClass::margin() - value_width;
                const int y = m_y + BaseClass::margin() + caption_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::textEngine().draw( p, min_value, BaseClass::screen() );
            }
            {
                const int x = BaseClass::x1() - BaseClass::margin() - value_width;
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::textEngine().draw( p, max_value, BaseClass::screen() );
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
void ColorMapBar::resizeEvent( int width, int height )
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
int ColorMapBar::adjustedWidth()
{
    size_t width = 0;
    switch ( m_orientation )
    {
    case ColorMapBar::Horizontal:
    {
        width = BaseClass::textEngine().width( m_caption ) + BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::ColorMapBarWidth );
        break;
    }
    case ColorMapBar::Vertical:
    {
        const std::string min_value = kvs::String::ToString( m_min_value );
        const std::string max_value = kvs::String::ToString( m_max_value );
        const size_t min_text_width = BaseClass::textEngine().width( min_value );
        const size_t max_text_width = BaseClass::textEngine().width( max_value );
        width = ( min_value.size() > max_value.size() ) ? min_text_width : max_text_width;
        width += BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::ColorMapBarHeight );
        break;
    }
    default: break;
    }

    return static_cast<int>( width );
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen height.
 *  @return screen height
 */
/*===========================================================================*/
int ColorMapBar::adjustedHeight()
{
    size_t height = 0;
    const size_t text_height = BaseClass::textEngine().height();
    switch( m_orientation )
    {
    case ColorMapBar::Horizontal:
        height = ::ColorMapBarHeight + ( text_height + BaseClass::margin() ) * 2;
        break;
    case ColorMapBar::Vertical:
        height = ::ColorMapBarWidth + text_height + BaseClass::margin() * 2;
        break;
    default: break;
    }

    return static_cast<int>( height );
}

/*===========================================================================*/
/**
 *  @brief  Create a texture for the color map.
 */
/*===========================================================================*/
void ColorMapBar::create_texture()
{
    const size_t nchannels = 3;
    const size_t width = m_colormap.resolution();
    const size_t height = 1;
    const kvs::UInt8* data = m_colormap.table().data();

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
void ColorMapBar::release_texture()
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
void ColorMapBar::draw_color_bar( const int x, const int y, const int width, const int height )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_BLEND );
    attrib.disable( GL_DEPTH_TEST );
    attrib.disable( GL_TEXTURE_3D );
    attrib.enable( GL_TEXTURE_2D );

    kvs::Texture::Binder binder( m_texture );
    switch ( m_orientation )
    {
    case ColorMapBar::Horizontal:
    {
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x,         y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x + width, y ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x + width, y + height ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x,         y + height ) );
        kvs::OpenGL::End();
        break;
    }
    case ColorMapBar::Vertical:
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
void ColorMapBar::draw_border( const int x, const int y, const int width, const int height )
{
    BaseClass::renderEngine().beginFrame( screen()->width(), screen()->height() );

    BaseClass::renderEngine().beginPath();
    BaseClass::renderEngine().setStrokeWidth( m_border_width );
    BaseClass::renderEngine().roundedRect( x - 0.5f, y + 2.0f, width + 1.0f, height, 3 );
    BaseClass::renderEngine().setStrokeColor( kvs::RGBAColor( 250, 250, 250, 0.6f ) );
    BaseClass::renderEngine().stroke();

    BaseClass::renderEngine().beginPath();
    BaseClass::renderEngine().setStrokeWidth( m_border_width );
    BaseClass::renderEngine().roundedRect( x - 0.5f, y, width + 1.0f, height, 3 );
    BaseClass::renderEngine().setStrokeColor( m_border_color );
    BaseClass::renderEngine().stroke();

    BaseClass::renderEngine().endFrame();
}

} // end of namespace kvs
