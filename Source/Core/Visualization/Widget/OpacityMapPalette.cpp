#include "OpacityMapPalette.h"
#include <kvs/MouseButton>
#include <kvs/MouseEvent>
#include <kvs/String>
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

OpacityMapPalette::OpacityMapPalette( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_palette( NULL )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::Margin );
    this->setCaption( "Opacity map palette " + kvs::String::ToString( ::InstanceCounter++ ) );

    m_opacity_map.create();
}

OpacityMapPalette::~OpacityMapPalette()
{
}

const kvs::OpacityMap OpacityMapPalette::opacityMap() const
{
    // Deep copy.
    kvs::OpacityMap::Table opacity_map_table( m_opacity_map.table().data(), m_opacity_map.table().size() );
    return kvs::OpacityMap( opacity_map_table );
}

void OpacityMapPalette::setOpacityMap( const kvs::OpacityMap& opacity_map )
{
    // Deep copy.
    kvs::OpacityMap::Table opacity_map_table( opacity_map.table().data(), opacity_map.table().size() );
    m_opacity_map = kvs::OpacityMap( opacity_map_table );
}

void OpacityMapPalette::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    if ( !m_texture.isValid() ) this->initialize_texture( m_opacity_map );
    if ( !m_checkerboard.isValid() ) this->initialize_checkerboard();

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    const int text_height = BaseClass::painter().fontMetrics().height();
    const int caption_height = ( m_caption.size() == 0 ) ? 0 : text_height;

    // Draw the caption.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin();
        BaseClass::painter().font().setStyleToBold();
        BaseClass::painter().drawText( kvs::Vec2( x, y + caption_height ), m_caption );
        BaseClass::painter().font().setStyleToRegular();
    }

    // Draw palette.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin() + caption_height + 5;
        const int width = BaseClass::width() - BaseClass::margin() * 2;
        const int height = BaseClass::height() - BaseClass::margin() * 2 - caption_height;
        m_palette.setGeometry( x, y, width, height );
    }

    this->draw_palette();

    BaseClass::painter().end();
}

void OpacityMapPalette::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

void OpacityMapPalette::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::contains( event->x(), event->y() ) )
    {
        BaseClass::screen()->disable();
        BaseClass::activate();

        if ( m_palette.contains( event->x(), event->y(), true ) )
        {
            m_palette.activate();

            // Opacity map palette geometry.
            const int x0 = m_palette.x0();
            const int x1 = m_palette.x1();
            const int y0 = m_palette.y0();
            const int y1 = m_palette.y1();

            // Current mouse cursor position.
            const int x = event->x();
            const int y = event->y();
            m_pressed_position.set( x, y );
            m_previous_position.set( x, y );

            // Opacity value.
            const float resolution = static_cast<float>( m_opacity_map.resolution() );
            const int index = static_cast<int>( ( x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
            const float opacity = static_cast<float>( y1 - y ) / ( y1 - y0 );

            // Update the opacity map.
            kvs::Real32* data = const_cast<kvs::Real32*>( m_opacity_map.table().data() );
            kvs::Real32* pdata = data;
            pdata = data + index;
            pdata[0] = opacity;

            // Download to GPU.
            const size_t width = m_opacity_map.resolution();
            m_texture.bind();
            m_texture.load( width, data );
            m_texture.unbind();
        }

        BaseClass::screen()->redraw();
    }
}

void OpacityMapPalette::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_palette.isActive() )
        {
            if ( event->button() == kvs::MouseButton::Left )
            {
                this->draw_free_hand_line( event );
            }
            else if ( event->button() == kvs::MouseButton::Right )
            {
                this->draw_straight_line( event );
            }
        }

        BaseClass::screen()->redraw();
    }
}

void OpacityMapPalette::mouseReleaseEvent( kvs::MouseEvent* event )
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

int OpacityMapPalette::adjustedWidth()
{
    size_t width = 0;
    BaseClass::painter().begin( BaseClass::screen() );
    width = BaseClass::painter().fontMetrics().width( m_caption ) + BaseClass::margin() * 2;
    BaseClass::painter().end();
    return kvs::Math::Max( width, ::Default::Width );
}

int OpacityMapPalette::adjustedHeight()
{
    size_t height = 0;
    BaseClass::painter().begin( BaseClass::screen() );
    height = BaseClass::painter().fontMetrics().height();
    BaseClass::painter().end();
    return ::Default::Height + height + BaseClass::margin() * 2;
}

void OpacityMapPalette::initialize_texture( const kvs::OpacityMap& opacity_map )
{
    const size_t width = opacity_map.resolution();
    const kvs::Real32* data = opacity_map.table().data();

    m_texture.release();
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.setPixelFormat( GL_ALPHA, GL_ALPHA, GL_FLOAT );
    m_texture.create( width, data );
}

void OpacityMapPalette::initialize_checkerboard()
{
    const size_t nchannels = 3;
    const int width = 32;
    const int height = 32;

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

void OpacityMapPalette::draw_palette()
{
    // Draw palette
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );

    const float dpr = screen()->devicePixelRatio();
    const int x0 = m_palette.x0();
    const int x1 = m_palette.x1();
    const int y0 = m_palette.y0();
    const int y1 = m_palette.y1();
    const float w = ( m_palette.width() / 32.0f );
    const float h = ( m_palette.height() / 32.0f );

    m_checkerboard.bind();
    kvs::OpenGL::Begin( GL_QUADS );
    {
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( w,    0.0f ), kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( w,    h    ), kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, h    ), kvs::Vec2( x0, y1 ) * dpr );
    }
    kvs::OpenGL::End();
    m_checkerboard.unbind();

    attrib.enable( GL_BLEND );
    attrib.enable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    kvs::OpenGL::SetBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA );

    // Draw opacity map.
    m_texture.bind();
    kvs::OpenGL::Begin( GL_QUADS );
    {
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x0, y1 ) * dpr );
    }
    kvs::OpenGL::End();
    m_texture.unbind();

    // Draw lines.
    const int width = m_palette.width();
    const int height = m_palette.height();
    const int resolution = m_opacity_map.resolution();
    const float stride_x = static_cast<float>( width ) / ( resolution - 1 );
    const kvs::Real32* data = m_opacity_map.table().data();
    const kvs::Vec2 range_min( static_cast<float>(x0), static_cast<float>(y0+1) );
    const kvs::Vec2 range_max( static_cast<float>(x1-1), static_cast<float>(y1) );

    attrib.enable( GL_LINE_SMOOTH );
    kvs::OpenGL::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    kvs::OpenGL::SetLineWidth( 1.5 * dpr );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    kvs::OpenGL::Color( kvs::RGBColor::Black() );
    for ( int i = 0; i < resolution; i++ )
    {
        const float x = kvs::Math::Clamp( x0 + i * stride_x,     range_min.x(), range_max.x() );
        const float y = kvs::Math::Clamp( y1 - height * data[i], range_min.y(), range_max.y() );
        kvs::OpenGL::Vertex( kvs::Vec2( x, y ) * dpr );
    }
    kvs::OpenGL::End();

    // Draw border
    this->draw_border( m_palette );
}

void OpacityMapPalette::draw_free_hand_line( kvs::MouseEvent* event )
{
    // Opacity map palette geometry.
    const int x0 = m_palette.x0();
    const int x1 = m_palette.x1();
    const int y0 = m_palette.y0();
    const int y1 = m_palette.y1();

    // Current mouse cursor position.
    const int x = event->x();
    const int y = event->y();

    const int old_x = kvs::Math::Clamp( m_previous_position.x(), x0, x1 );
    const int old_y = kvs::Math::Clamp( m_previous_position.y(), y0, y1 );
    const int new_x = kvs::Math::Clamp( x,  x0, x1 );
    const int new_y = kvs::Math::Clamp( y,  y0, y1 );

    const float old_opacity = static_cast<float>( y1 - old_y ) / ( y1 - y0 );
    const float new_opacity = static_cast<float>( y1 - new_y ) / ( y1 - y0 );
    const float diff_opacity = new_opacity - old_opacity;

    const float resolution = static_cast<float>( m_opacity_map.resolution() );
    const int old_index = static_cast<int>( ( old_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
    const int new_index = static_cast<int>( ( new_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
    const float diff_index = static_cast<float>( new_index - old_index );

    // Update the opacity map.
    const int begin_index = kvs::Math::Min( old_index, new_index );
    const int end_index = kvs::Math::Max( old_index, new_index );
    kvs::Real32* data = const_cast<kvs::Real32*>( m_opacity_map.table().data() );
    kvs::Real32* pdata = data + begin_index;
    for ( int i = begin_index; i < end_index; i++ )
    {
        const float opacity = ( i - old_index ) * diff_opacity / diff_index + old_opacity;
        *(pdata++) = opacity;
    }

    // Download to GPU.
    const size_t width = m_opacity_map.resolution();
    m_texture.bind();
    m_texture.load( width, data );
    m_texture.unbind();

    // Update the previous mouse position.
    m_previous_position.set( x, y );
}

void OpacityMapPalette::draw_straight_line( kvs::MouseEvent* event )
{
    // Opacity map palette geometry.
    const int x0 = m_palette.x0();
    const int x1 = m_palette.x1();
    const int y0 = m_palette.y0();
    const int y1 = m_palette.y1();

    // Current mouse cursor position.
    const int x = event->x();
    const int y = event->y();

    const int old_x = kvs::Math::Clamp( m_previous_position.x(), x0, x1 );
    const int old_y = kvs::Math::Clamp( m_previous_position.y(), y0, y1 );
    const int new_x = kvs::Math::Clamp( x,  x0, x1 );
    const int new_y = kvs::Math::Clamp( y,  y0, y1 );
    const int fix_x = kvs::Math::Clamp( m_pressed_position.x(), x0, x1 );
    const int fix_y = kvs::Math::Clamp( m_pressed_position.y(), y0, y1 );

    const float old_opacity = static_cast<float>( y1 - old_y ) / ( y1 - y0 );
    const float new_opacity = static_cast<float>( y1 - new_y ) / ( y1 - y0 );
    const float fix_opacity = static_cast<float>( y1 - fix_y ) / ( y1 - y0 );

    const float resolution = static_cast<float>( m_opacity_map.resolution() );
    const int old_index = static_cast<int>( ( old_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
    const int new_index = static_cast<int>( ( new_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );
    const int fix_index = static_cast<int>( ( fix_x - x0 ) * resolution / ( x1 - x0 ) + 0.5f );

    // Update the opacity map.
    kvs::Real32* data = const_cast<kvs::Real32*>( m_opacity_map.table().data() );
    if ( ( new_x - old_x ) * ( fix_x - old_x ) < 0 )
    {
        // Straight line.
        const float diff_opacity = new_opacity - fix_opacity;
        const float diff_index = static_cast<float>( new_index - fix_index );

        const int begin_index = kvs::Math::Min( fix_index, new_index );
        const int end_index = kvs::Math::Max( fix_index, new_index );
        kvs::Real32* pdata = data + begin_index;
        for ( int i = begin_index; i < end_index; i++ )
        {
            const float opacity = ( i - fix_index ) * diff_opacity / diff_index + fix_opacity;
            *(pdata++) = opacity;
        }
    }
    else
    {
        // Straight line.
        {
            const float diff_opacity = old_opacity - fix_opacity;
            const float diff_index = static_cast<float>( old_index - fix_index );

            const int begin_index = kvs::Math::Min( fix_index, old_index );
            const int end_index = kvs::Math::Max( fix_index, old_index );
            kvs::Real32* pdata = data + begin_index;
            for ( int i = begin_index; i < end_index; i++ )
            {
                const float opacity = ( i - fix_index ) * diff_opacity / diff_index + fix_opacity;
                *(pdata++) = opacity;
            }
        }
        // Free-hand line.
        {
            const float diff_opacity = new_opacity - old_opacity;
            const float diff_index = static_cast<float>( new_index - old_index );

            const int begin_index = kvs::Math::Min( old_index, new_index );
            const int end_index = kvs::Math::Max( old_index, new_index );
            kvs::Real32* data = const_cast<kvs::Real32*>( m_opacity_map.table().data() );
            kvs::Real32* pdata = data + begin_index;
            for ( int i = begin_index; i < end_index; i++ )
            {
                const float opacity = ( i - old_index ) * diff_opacity / diff_index + old_opacity;
                *(pdata++) = opacity;
            }
        }
    }

    // Download to GPU.
    const size_t width = m_opacity_map.resolution();
    m_texture.bind();
    m_texture.load( width, data );
    m_texture.unbind();

    // Update the previous mouse position.
    m_previous_position.set( x, y );
}

void OpacityMapPalette::draw_border( const kvs::Rectangle& rect )
{
    const int x = rect.x();
    const int y = rect.y();
    const int width = rect.width();
    const int height = rect.height();
    const float border_width = 1.0f;
    const kvs::RGBColor border_color = kvs::RGBColor::Black();

    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), screen()->devicePixelRatio() );

    engine->beginPath();
    engine->setStrokeWidth( border_width );
    engine->roundedRect( x - 0.5f, y + 2.0f, width + 1.0f, height, 3 );
    engine->setStrokeColor( kvs::RGBAColor( 250, 250, 250, 0.6f ) );
    engine->stroke();

    engine->beginPath();
    engine->setStrokeWidth( border_width );
    engine->roundedRect( x - 0.5f, y, width + 1.0f, height, 3 );
    engine->setStrokeColor( border_color );
    engine->stroke();

    engine->endFrame();
}

} // end of namespace kvs
