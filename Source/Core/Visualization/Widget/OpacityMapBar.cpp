/*****************************************************************************/
/**
 *  @file   OpacityMapBar.cpp
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
#include "OpacityMapBar.h"
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
const size_t OpacityMapBarWidth = 150;
const size_t OpacityMapBarHeight = 30;
const size_t OpacityMapBarMargin = 10;
}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new OpacityMapBar class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
OpacityMapBar::OpacityMapBar( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_show_range_value( true )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );

    BaseClass::setMargin( ::OpacityMapBarMargin );
    this->setCaption( "" );
    this->setOrientation( OpacityMapBar::Horizontal );
    this->setNumberOfDivisions( 5 );
    this->setDivisionLineWidth( 1.0f );
    this->setDivisionLineColor( kvs::RGBColor( 0, 0, 0 ) );
    this->setRange( ::MinValue, ::MaxValue );
    this->setBorderWidth( 1.0f );
    this->setBorderColor( kvs::RGBColor( 0, 0, 0 ) );
    this->disableAntiAliasing();

    m_opacity_map.setResolution( 256 );
    m_opacity_map.create();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the OpacityMapBar class.
 */
/*===========================================================================*/
OpacityMapBar::~OpacityMapBar()
{
}

void OpacityMapBar::setOpacityMap( const kvs::OpacityMap& opacity_map )
{
    // Deep copy.
    kvs::OpacityMap::Table opacity_table( opacity_map.table().data(), opacity_map.table().size() );
    m_opacity_map = kvs::OpacityMap( opacity_table );

    if ( opacity_map.hasRange() )
    {
        m_min_value = opacity_map.minValue();
        m_max_value = opacity_map.maxValue();
    }
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void OpacityMapBar::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) { return; }
    if ( !m_texture.isValid() ) { this->create_texture(); }
    if ( !m_checkerboard.isValid() ) { this->create_checkerboard(); }

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const std::string min_value = kvs::String::ToString( m_min_value );
    const std::string max_value = kvs::String::ToString( m_max_value );
    const int text_height = metrics.height();
    const int min_text_width = metrics.width( min_value );
    const int max_text_width = metrics.width( max_value );
    const int caption_height = ( m_caption.size() == 0 ) ? 0 : text_height + 5;
    const int value_width = ( min_value.size() > max_value.size() ) ? min_text_width : max_text_width;
    const int value_height = ( m_show_range_value ) ? text_height : 0;

    // Draw the color bar.
    {
        const int w = ( m_orientation == OpacityMapBar::Vertical ) ? value_width + 5 : 0;
        const int h = ( m_orientation == OpacityMapBar::Vertical ) ? 0 : value_height;
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin() + caption_height;
        const int width = BaseClass::width() - BaseClass::margin() * 2 - w;
        const int height = BaseClass::height() - BaseClass::margin() * 2 - caption_height - h;
        this->draw_opacity_bar( x, y, width, height );
        this->draw_border( x, y, width, height );
    }

    // Draw the caption.
    if ( m_caption.size() != 0 )
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin();
        const kvs::Vec2 p( x, y + text_height );
        BaseClass::painter().drawText( p, m_caption );
    }

    // Draw the values.
    if ( m_show_range_value )
    {
        switch ( m_orientation )
        {
        case OpacityMapBar::Horizontal:
        {
            {
                const int x = BaseClass::x0() + BaseClass::margin();
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::painter().drawText( p, min_value );
            }
            {
                const int x = BaseClass::x1() - BaseClass::margin() - max_text_width;
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::painter().drawText( p, max_value );
            }
            break;
        }
        case OpacityMapBar::Vertical:
        {
            {
                const int x = BaseClass::x1() - BaseClass::margin() - value_width;
                const int y = BaseClass::y0() + BaseClass::margin() + caption_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::painter().drawText( p, min_value );
            }
            {
                const int x = BaseClass::x1() - BaseClass::margin() - value_width;
                const int y = BaseClass::y1() - BaseClass::margin() - text_height;
                const kvs::Vec2 p( x, y + text_height );
                BaseClass::painter().drawText( p, max_value );
            }
            break;
        }
        default: break;
        }
    }

    BaseClass::painter().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void OpacityMapBar::resizeEvent( int width, int height )
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
int OpacityMapBar::adjustedWidth()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();

    size_t width = 0;
    switch ( m_orientation )
    {
    case OpacityMapBar::Horizontal:
    {
        width = metrics.width( m_caption ) + BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::OpacityMapBarWidth );
        break;
    }
    case OpacityMapBar::Vertical:
    {
        const std::string min_value = kvs::String::ToString( m_min_value );
        const std::string max_value = kvs::String::ToString( m_max_value );
        const size_t min_text_width = metrics.width( min_value );
        const size_t max_text_width = metrics.width( max_value );
        width = ( min_value.size() > max_value.size() ) ? min_text_width : max_text_width;
        width += BaseClass::margin() * 2;
        width = kvs::Math::Max( width, ::OpacityMapBarHeight );
        break;
    }
    default: break;
    }
    BaseClass::painter().end();

    return static_cast<int>( width );
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial screen height.
 *  @return screen height
 */
/*===========================================================================*/
int OpacityMapBar::adjustedHeight()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t text_height = metrics.height();
    BaseClass::painter().end();

    size_t height = 0;
    switch( m_orientation )
    {
    case OpacityMapBar::Horizontal:
        height = ::OpacityMapBarHeight + ( text_height + BaseClass::margin() ) * 2;
        break;
    case OpacityMapBar::Vertical:
        height = ::OpacityMapBarWidth + text_height + BaseClass::margin() * 2;
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
void OpacityMapBar::create_texture()
{
    const size_t nchannels = 1;
    const size_t width = m_opacity_map.resolution();
    const size_t height = 1;
    const kvs::Real32* data = m_opacity_map.table().data();

    m_texture.release();
    m_texture.setPixelFormat( nchannels, sizeof( kvs::Real32 ) );
    m_texture.setMinFilter( GL_NEAREST );
    m_texture.setMagFilter( GL_NEAREST );
    m_texture.create( width, height, data );
}

void OpacityMapBar::create_checkerboard()
{
    const size_t nchannels = 3;
    const int width = 16;
    const int height = 16;

    GLubyte* data = new GLubyte [ width * height * nchannels ];
    if ( !data )
    {
        kvsMessageError("Cannot allocate for the checkerboard texture.");
        return;
    }

    GLubyte* pdata = data;
    const int c1 = 255; // checkerboard color (gray value) 1
    const int c2 = 230; // checkerboard color (gray value) 2
    for ( int i = 0; i < height; i++ )
    {
        for ( int j = 0; j < width; j++ )
        {
            int c = ((((i&0x8)==0)^((j&0x8)==0))) * c1;
            c = ( c == 0 ) ? c2 : c;
            *(pdata++) = static_cast<GLubyte>(c);
            *(pdata++) = static_cast<GLubyte>(c);
            *(pdata++) = static_cast<GLubyte>(c);
        }
    }

    m_checkerboard.release();
    m_checkerboard.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
    m_checkerboard.setMinFilter( GL_NEAREST );
    m_checkerboard.setMagFilter( GL_NEAREST );
    m_checkerboard.setWrapS( GL_REPEAT );
    m_checkerboard.setWrapT( GL_REPEAT );
    m_checkerboard.create( width, height, data );

    delete [] data;
}

/*===========================================================================*/
/**
 *  @brief  Draws the opacity bar.
 *  @param  x [in] x position of the bar
 *  @param  y [in] y position of the bar
 *  @param  width [in] bar width
 *  @param  height [in] bar height
 */
/*===========================================================================*/
void OpacityMapBar::draw_opacity_bar( const int x, const int y, const int width, const int height )
{
    const float dpr = screen()->devicePixelRatio();
    const int x0 = x;
    const int x1 = x + width;
    const int y0 = y;
    const int y1 = y + height;

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );

    // Draw checkerboard.
    {
        const float aspect = float(width) / float(height);
        const float ratio = aspect >= 1.0f ? aspect : ( 1.0f / aspect );
        const float w = 2.0f * ratio * ( width < height ? aspect : 1.0f );
        const float h = 2.0f * ratio * ( width < height ? 1.0f : aspect );

        kvs::Texture::Binder binder( m_checkerboard );
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2(    w, 1.0f ), kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2(    w,    h ), kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f,    h ), kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }

    attrib.enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA );

    kvs::Texture::Binder binder( m_texture );
    switch ( m_orientation )
    {
    case OpacityMapBar::Horizontal:
    {
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
        break;
    }
    case OpacityMapBar::Vertical:
    {
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x0, y1 ) * dpr );
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
void OpacityMapBar::draw_border( const int x, const int y, const int width, const int height )
{
    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), screen()->devicePixelRatio() );

    engine->beginPath();
    engine->setStrokeWidth( m_border_width );
    engine->roundedRect( x - 0.5f, y + 2.0f, width + 1.0f, height, 3 );
    engine->setStrokeColor( kvs::RGBAColor( 250, 250, 250, 0.6f ) );
    engine->stroke();

    engine->beginPath();
    engine->setStrokeWidth( m_border_width );
    engine->roundedRect( x - 0.5f, y, width + 1.0f, height, 3 );
    engine->setStrokeColor( m_border_color );
    engine->stroke();

    engine->endFrame();
}

} // end of namespace kvs
