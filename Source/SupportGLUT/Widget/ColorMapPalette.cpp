/*****************************************************************************/
/**
 *  @file   ColorMapPalette.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorMapPalette.cpp 1551 2013-04-20 01:40:09Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ColorMapPalette.h"
#include <cstdio>
#include <kvs/String>
#include <kvs/EventBase>
#include <kvs/MouseEvent>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>

// Default parameters.
namespace { namespace Default
{
const size_t Margin = 10;
const size_t Width = 350;
const size_t Height = 50;
const kvs::RGBColor RectColor = kvs::RGBColor( 255, 255, 255 );
const kvs::RGBColor RectEdgeColor = kvs::RGBColor( 230, 230, 230 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ColorMapPalette class.
 *  @param  screen [in] pointer to parent screen
 */
/*===========================================================================*/
ColorMapPalette::ColorMapPalette( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_palette( NULL ),
    m_color_palette( NULL )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::Margin );
    this->setCaption( "Color map palette " + kvs::String::ToString( ::InstanceCounter++ ) );

    m_upper_edge_color = BaseClass::darkenedColor( ::Default::RectColor, 0.6f );
    m_lower_edge_color = ::Default::RectEdgeColor;
    m_drawing_color = kvs::RGBColor( 0, 0, 0 );

    m_color_map.create();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ColorMapPalette class.
 */
/*===========================================================================*/
ColorMapPalette::~ColorMapPalette()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the color map.
 *  @return color map
 */
/*===========================================================================*/
const kvs::ColorMap ColorMapPalette::colorMap() const
{
    kvs::ColorMap::Table color_map_table( m_color_map.table().data(), m_color_map.table().size() );
    return kvs::ColorMap( color_map_table );
}

/*===========================================================================*/
/**
 *  @brief  Sets a color map.
 *  @param  color_map [in] color map
 */
/*===========================================================================*/
void ColorMapPalette::setColorMap( const kvs::ColorMap& color_map )
{
    // Deep copy.
    kvs::ColorMap::Table color_map_table( color_map.table().data(), color_map.table().size() );
    m_color_map = kvs::ColorMap( color_map_table );
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void ColorMapPalette::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    if ( !m_texture.isValid() ) { this->initialize_texture( m_color_map ); }

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    // Draw the caption.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin();
        BaseClass::drawText( x, y + BaseClass::characterHeight(), m_caption );
    }

    // Draw palette.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + 5;
        const int width = BaseClass::width() - BaseClass::margin() * 2;
        const int height = BaseClass::height() - BaseClass::margin() * 2 - BaseClass::characterHeight();
        m_palette.setGeometry( x, y, width, height );
    }

    this->draw_palette();

    BaseClass::render2D().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] width
 *  @param  height [in] height
 */
/*===========================================================================*/
void ColorMapPalette::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to mouse event
 */
/*===========================================================================*/
void ColorMapPalette::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::contains( event->x(), event->y() ) )
    {
        BaseClass::screen()->disable();
        BaseClass::activate();

        if ( m_palette.contains( event->x(), event->y(), true ) )
        {
            m_palette.activate();

            // Color map palette geometry.
            const int x0 = m_palette.x0();
            const int x1 = m_palette.x1();
            const int y0 = m_palette.y0();
            const int y1 = m_palette.y1();

            // Current mouse cursor position.
            const int x = event->x();
            const int y = event->y();
            m_pressed_position.set( x, y );

            const float resolution = static_cast<float>( m_color_map.resolution() );
            const float ratio = static_cast<float>( y1 - y ) / ( y1 - y0 );
            const int index = static_cast<int>( ( x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );

            // Update the color data.
            if ( m_color_palette ) m_drawing_color = m_color_palette->color();
            const kvs::RGBColor drawing_color = m_drawing_color;
            kvs::UInt8* data = const_cast<kvs::UInt8*>( m_color_map.table().data() );
            kvs::UInt8* pdata = data;
            pdata = data + index * 3;
            pdata[0] = static_cast<kvs::UInt8>( drawing_color.r() * ratio + pdata[0] * ( 1 - ratio ) );
            pdata[1] = static_cast<kvs::UInt8>( drawing_color.g() * ratio + pdata[1] * ( 1 - ratio ) );
            pdata[2] = static_cast<kvs::UInt8>( drawing_color.b() * ratio + pdata[2] * ( 1 - ratio ) );

            const size_t width = m_color_map.resolution();
            m_texture.bind();
            m_texture.load( width, data );
            m_texture.unbind();
        }

        BaseClass::screen()->redraw();
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  event [in] pointer to mouse event
 */
/*===========================================================================*/
void ColorMapPalette::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_palette.isActive() )
        {
            // Color map palette geometry.
            const int x0 = m_palette.x0();
            const int x1 = m_palette.x1();
            const int y0 = m_palette.y0();
            const int y1 = m_palette.y1();

            // Current mouse cursor position.
            const int x = event->x();
            const int y = event->y();
            const int old_x = kvs::Math::Clamp( m_pressed_position.x(), x0, x1 );
            const int old_y = kvs::Math::Clamp( m_pressed_position.y(), y0, y1 );
            const int new_x = kvs::Math::Clamp( x,  x0, x1 );
            const int new_y = kvs::Math::Clamp( y,  y0, y1 );
            m_pressed_position.set( x, y );

            const float old_ratio = static_cast<float>( y1 - old_y ) / ( y1 - y0 );
            const float new_ratio = static_cast<float>( y1 - new_y ) / ( y1 - y0 );
            const float diff_ratio = new_ratio - old_ratio;

            const float resolution = static_cast<float>( m_color_map.resolution() );
            const int old_index = static_cast<int>( ( old_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
            const int new_index = static_cast<int>( ( new_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
            const float diff_index = static_cast<float>( new_index - old_index );

            if ( m_color_palette ) m_drawing_color = m_color_palette->color();
            const kvs::RGBColor drawing_color = m_drawing_color;
            const int begin_index = kvs::Math::Min( old_index, new_index );
            const int end_index = kvs::Math::Max( old_index, new_index );
            kvs::UInt8* data = const_cast<kvs::UInt8*>( m_color_map.table().data() );
            kvs::UInt8* pdata = data + begin_index * 3;
            for ( int i = begin_index; i < end_index; i++, pdata += 3 )
            {
                const float ratio = ( i - old_index ) * diff_ratio / diff_index + old_ratio;
                pdata[0] = static_cast<kvs::UInt8>( drawing_color.r() * ratio + pdata[0] * ( 1 - ratio ) );
                pdata[1] = static_cast<kvs::UInt8>( drawing_color.g() * ratio + pdata[1] * ( 1 - ratio ) );
                pdata[2] = static_cast<kvs::UInt8>( drawing_color.b() * ratio + pdata[2] * ( 1 - ratio ) );
            }

            const size_t width = m_color_map.resolution();
            m_texture.bind();
            m_texture.load( width, data );
            m_texture.unbind();
        }

        BaseClass::screen()->redraw();
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to mouse event
 */
/*===========================================================================*/
void ColorMapPalette::mouseReleaseEvent( kvs::MouseEvent* event )
{
    kvs::IgnoreUnusedVariable( event );

    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_palette.isActive() ) m_palette.deactivate();

        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

int ColorMapPalette::adjustedWidth()
{
    const size_t width = m_caption.size() * BaseClass::characterWidth() + BaseClass::margin() * 2;
    return kvs::Math::Max( width, ::Default::Width );
}

int ColorMapPalette::adjustedHeight()
{
    return ::Default::Height + BaseClass::characterHeight() + BaseClass::margin() * 2;
}

void ColorMapPalette::initialize_texture( const kvs::ColorMap& color_map )
{
    const size_t nchannels  = 3; // rgb
    const size_t width = color_map.resolution();
    const kvs::UInt8* data = color_map.table().data();

    m_texture.release();
    m_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.create( width, data );
}

void ColorMapPalette::draw_palette()
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_BLEND );
    attrib.enable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );

    const int x0 = m_palette.x0();
    const int x1 = m_palette.x1();
    const int y0 = m_palette.y0();
    const int y1 = m_palette.y1();

    // Draw color map texture.
    m_texture.bind();
    kvs::OpenGL::Begin( GL_QUADS );
    {
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y0 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x1, y0 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x1, y1 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x0, y1 ) );
    }
    kvs::OpenGL::End();
    m_texture.unbind();

    // Draw border.
    BaseClass::drawRect( m_palette, m_upper_edge_color, m_lower_edge_color );
/*
    kvs::OpenGL::SetLineWidth( 1 );
    kvs::OpenGL::Begin( GL_LINES );
    {
        kvs::OpenGL::Color( m_upper_edge_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x0, y0 ), kvs::Vec2( x1, y0 ) ); // top
        kvs::OpenGL::Vertices( kvs::Vec2( x0, y0 ), kvs::Vec2( x0, y1 ) ); // left
        kvs::OpenGL::Color( m_lower_edge_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x1, y1 ), kvs::Vec2( x0, y1 ) ); // bottom
        kvs::OpenGL::Vertices( kvs::Vec2( x1, y0 ), kvs::Vec2( x1, y1 ) ); // right
    }
    kvs::OpenGL::End();
*/
}

} // end of namespace glut

} // end of namespace kvs
