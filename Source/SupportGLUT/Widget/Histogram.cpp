/****************************************************************************/
/**
 *  @file Histogram.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Histogram.cpp 1690 2014-01-01 08:14:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Histogram.h"
#include <kvs/StructuredVolumeObject>
#include <kvs/String>
#include <kvs/MouseButton>
#include <kvs/MouseEvent>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include <kvs/glut/GLUT>
#include <kvs/OpenGL>


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

namespace
{

/*
const kvs::Real32 GaussianKernel( const kvs::Real32 x )
{
    return std::exp( ( -0.5f ) * x * x ) / std::sqrt( 2.0f * M_PI );
}
*/

} // end of namespace


namespace kvs
{

namespace glut
{


/*===========================================================================*/
/**
 *  @brief  Constructs a new Histogram class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
Histogram::Histogram( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_graph_color( kvs::RGBAColor( 0, 0, 0, 1.0f ) ),
    m_bias_parameter( 0.5f ),
    m_palette( NULL )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::Margin );
    this->setCaption( "Histogram " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setNumberOfBins( 256 );

    m_upper_edge_color = BaseClass::darkenedColor( ::Default::RectColor, 0.6f );
    m_lower_edge_color = ::Default::RectEdgeColor;
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Histogram class.
 */
/*===========================================================================*/
Histogram::~Histogram()
{
}

/*==========================================================================*/
/**
 *  @brief  Create a frequency distribution table for volume object.
 *  @param  volume [in] pointer to volume object
 */
/*==========================================================================*/
void Histogram::create( const kvs::VolumeObjectBase* volume )
{
    volume->updateMinMaxValues();
    m_table.create( volume );
//    this->calculate_density_curve();
}

/*==========================================================================*/
/**
 *  @brief  Create a frequency distribution table for image object
 *  @param  image [in] pointer to image object
 */
/*==========================================================================*/
void Histogram::create( const kvs::ImageObject* image )
{
    m_table.create( image );
}

void Histogram::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    if ( !m_texture.isValid() ) this->create_texture();

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

void Histogram::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

void Histogram::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::contains( event->x(), event->y() ) )
    {
        BaseClass::screen()->disable();
        BaseClass::activate();

        if ( m_palette.contains( event->x(), event->y(), true ) )
        {
            m_palette.activate();

            // Current mouse cursor position.
            const int x = event->x();
            const int y = event->y();
            m_previous_position.set( x, y );
        }

        BaseClass::screen()->redraw();
    }
}

void Histogram::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_palette.isActive() )
        {
            const int x = event->x();
            const int y = event->y();
            const int dy = y - m_previous_position.y();

            // Calculate the bias parameter.
            m_bias_parameter -= dy * 0.005f;
            m_bias_parameter = kvs::Math::Clamp( m_bias_parameter, 0.0f, 0.99999f );
            this->update_texture();

            m_previous_position.set( x, y );
        }

        BaseClass::screen()->redraw();
    }
}

void Histogram::mouseReleaseEvent( kvs::MouseEvent* event )
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

int Histogram::adjustedWidth()
{
    const size_t width = m_caption.size() * BaseClass::characterWidth() + BaseClass::margin() * 2;
    return kvs::Math::Max( width, ::Default::Width );
}

int Histogram::adjustedHeight()
{
    return ::Default::Height + BaseClass::characterHeight() + BaseClass::margin() * 2;
}

/*==========================================================================*/
/**
 *  @brief  Draws the frequency distribution graph.
 */
/*==========================================================================*/
void Histogram::draw_palette()
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.enable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );

    const int x0 = m_palette.x0();
    const int x1 = m_palette.x1();
    const int y0 = m_palette.y0();
    const int y1 = m_palette.y1();
    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( kvs::RGBColor( 240, 240, 240 ) );
    kvs::OpenGL::Vertex( x0, y0 );
    kvs::OpenGL::Vertex( x1, y0 );
    kvs::OpenGL::Vertex( x1, y1 );
    kvs::OpenGL::Vertex( x0, y1 );
    kvs::OpenGL::End();

    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    if ( m_texture.isValid() )
    {
        kvs::Texture::Binder binder( m_texture );
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( x0, y1 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( x1, y1 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( x1, y0 ) );
        kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( x0, y0 ) );
        kvs::OpenGL::End();
    }
    kvs::OpenGL::Disable( GL_BLEND );

/*
    if ( m_density_curve.size() > 0 )
    {
        const float g = m_bias_parameter;
        const float scale_width = static_cast<float>( m_palette.width() ) / m_table.numberOfBins();
        const float scale_height = static_cast<float>( m_palette.height() );
        glBegin( GL_LINE_STRIP );
        glColor3ub( 255, 0, 0 );
        for ( size_t i = 0; i < m_density_curve.size(); i++ )
        {
            const float x = m_palette.x0() + i * scale_width;
            const float y = m_palette.y0() + m_density_curve[i] * scale_height;
            glVertex2f( x, y );
        }
        glEnd();
    }
*/

    // Draw border.
    BaseClass::drawRect( m_palette, m_upper_edge_color, m_lower_edge_color, 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the histogram image.
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::UInt8> Histogram::get_histogram_image() const
{
    const size_t nchannels = 4;
    const size_t width = m_table.numberOfBins();
    const size_t height = width;
    const size_t npixels = width * height;

    // Histogram image data.
    kvs::ValueArray<kvs::UInt8> data( npixels * nchannels );
    data.fill( 0 );

    if ( m_table.bin().size() == width )
    {
        const float g = kvs::Math::Clamp( m_bias_parameter, 0.0f, 1.0f );
        const kvs::Real32 normalized_factor = 1.0f / m_table.maxCount();
        for ( size_t i = 0; i < width; i++ )
        {
            // Calculate bias parameter.
            // Bias function: b(f,g) = f^{ln(g)/ln(0.5)}
            //  f: frequecny count that is normalized in [0,1]
            //  g: bias parameter in [0,1]
            const size_t n = m_table.bin().at(i); // frequency count
            const float f = n * normalized_factor; // normalized frequency count in [0,1]
            const float b = std::pow( f, static_cast<float>( std::log(g) / std::log(0.5) ) );

            const size_t h = static_cast<size_t>( b * height + 0.5f );
            for ( size_t j = 0; j < h; j++ )
            {
                const size_t index = i + j * width;
                data[ 4 * index + 0 ] = m_graph_color.r();
                data[ 4 * index + 1 ] = m_graph_color.g();
                data[ 4 * index + 2 ] = m_graph_color.b();
                data[ 4 * index + 3 ] = static_cast<kvs::UInt8>( m_graph_color.a() * 255.0f );
            }
        }
    }

    return data;
}

/*
void Histogram::calculate_density_curve()
{
    // Temporary array (biased histogram).
    const size_t width = m_table.numberOfBins();
    kvs::ValueArray<kvs::Real32> temp( width );
    temp.fill( 0 );

    const kvs::Real32 normalized_factor = 1.0f / m_table.maxCount();
    for ( size_t index = 0; index < width; index++ )
    {
        // Calculate bias parameter.
        const size_t n = m_table.bin().at( index ); // frequency count
        const float f = n * normalized_factor; // normalized frequency count in [0,1]
        temp[index] = f;
//        temp[index] = n;
    }

    // Density curve.
    m_density_curve.allocate( width );
    m_density_curve.fill( 0 );

    const float h = 0.9 / std::pow( width, 0.2 ); // band width

    kvs::Real32 max_density = 0;
    for ( size_t i = 0; i < width; i++ )
    {
        float sum = 0;
        const float xi = temp[i];
        for ( size_t j = 0; j < width; j++ )
        {
            const float xj = temp[j];
            sum += ::GaussianKernel( ( xi - xj ) / h );
        }

        m_density_curve[i] = sum / ( width * h );
        max_density = kvs::Math::Max( max_density, m_density_curve[i] );
    }

    for ( size_t i = 0; i < width; i++ ) m_density_curve[i] /= max_density;
}
*/

/*===========================================================================*/
/**
 *  @brief  Creates the histogram texture.
 */
/*===========================================================================*/
void Histogram::create_texture()
{
    const size_t nchannels = 4;
    const size_t width = m_table.numberOfBins();
    const size_t height = width;

    m_texture.release();
    m_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.create( width, height, this->get_histogram_image().data() );
}

/*===========================================================================*/
/**
 *  @brief  Updates the histogram texture.
 */
/*===========================================================================*/
void Histogram::update_texture()
{
    this->create_texture();
}

} // end of namesapce glut

} // end of namespace kvs
