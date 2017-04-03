/*****************************************************************************/
/**
 *  @file   Histogram.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Histogram.cpp 1690 2014-01-01 08:14:08Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Histogram.h"
#include <string>
#include <kvs/DebugNew>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/FrequencyTable>
#include <kvs/MouseButton>
#include <kvs/Key>
#include <kvs/MouseEvent>
#include <kvs/KeyEvent>
#include <kvs/InitializeEventListener>
#include <kvs/PaintEventListener>
#include <kvs/MousePressEventListener>
#include <kvs/MouseMoveEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Background>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include "CommandName.h"
#include "FileChecker.h"


namespace
{
// Default parameters.
const kvs::RGBColor DefaultBackgroundColor( 212, 221, 229 );
const kvs::RGBColor DefaultHistogramColor( 100, 100, 100 );
const float DefaultBiasParameter( 0.5f );
const size_t DefaultScreenWidth( 512 );
const size_t DefaultScreenHeight( 150 );
}

namespace kvsview
{

namespace Histogram
{

/*===========================================================================*/
/**
 *  @brief  Parameter set.
 */
/*===========================================================================*/
struct Parameters
{
    kvs::Vector2i mouse; ///< mouse position for 2D operation
    kvs::FrequencyTable frequency_table; ///< frequency table
    float bias_parameter; ///< bias parameter [0,1]
    kvs::Texture2D histogram_texture; ///< histogram texture

    Parameters( Argument& arg )
    {
        bias_parameter = arg.biasParameter();
    }

    bool createFrequencyTable( const std::string& filename )
    {
        if ( kvsview::FileChecker::ImportableStructuredVolume( filename ) )
        {
            kvs::StructuredVolumeObject* object = new kvs::StructuredVolumeImporter( filename );
            if ( !object )
            {
                kvsMessageError("Cannot import a structured volume object.");
                return false;
            }

            object->updateMinMaxValues();

            this->frequency_table.create( object );
        }
        else if ( kvsview::FileChecker::ImportableUnstructuredVolume( filename ) )
        {
            kvs::UnstructuredVolumeObject* object = new kvs::UnstructuredVolumeImporter( filename );
            if ( !object )
            {
                kvsMessageError("Cannot import a unstructured volume object.");
                return false;
            }

            object->updateMinMaxValues();

            this->frequency_table.create( object );
        }

        return true;
    }

    void createHistogramTexture()
    {
        const size_t nchannels = 4;
        const size_t width = static_cast<size_t>( this->frequency_table.numberOfBins() );
        const size_t height = width;

        this->histogram_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
        this->histogram_texture.create( width, height );
        this->histogram_texture.load( width, height, this->histogramImage().data() );
    }

    void updateHistogramTexture()
    {
        this->histogram_texture.release();
        this->createHistogramTexture();
    }

    const kvs::ValueArray<kvs::UInt8> histogramImage()
    {
        const size_t nchannels = 4;
        const size_t width = static_cast<size_t>( frequency_table.numberOfBins() );
        const size_t height = width;
        const size_t npixels = width * height;

        kvs::ValueArray<kvs::UInt8> data( npixels * nchannels );
        data.fill( 0 );

        const float g = bias_parameter; // bias parameter
        const kvs::Real32 normalized_factor = 1.0f / ( frequency_table.maxCount() );
        for ( size_t i = 0; i < width; i++ )
        {
            const size_t n = frequency_table.bin().at(i); // frequency count
            const float  f = n * normalized_factor; // normalized frequency count in [0,1]
            const float  b = std::pow( f, static_cast<float>( std::log(g) / std::log(0.5) ) );

            const size_t h = static_cast<size_t>( b * height + 0.5f );
            for ( size_t j = 0; j < h; j++ )
            {
                const size_t index = i + j * width;
                data[ 4 * index + 0 ] = ::DefaultHistogramColor.r();
                data[ 4 * index + 1 ] = ::DefaultHistogramColor.g();
                data[ 4 * index + 2 ] = ::DefaultHistogramColor.b();
                data[ 4 * index + 3 ] = kvs::UInt8( 255 );
            }
        }

        return data;
    }
};

/*===========================================================================*/
/**
 *  @brief  Initialize event.
 */
/*===========================================================================*/
class InitializeEvent : public kvs::InitializeEventListener
{
private:
    kvsview::Histogram::Parameters* m_parameters;

public:
    InitializeEvent( kvsview::Histogram::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update()
    {
        m_parameters->createHistogramTexture();
    }
};

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
class PaintEvent : public kvs::PaintEventListener
{
private:
    kvsview::Histogram::Parameters* m_parameters;

public:
    PaintEvent( kvsview::Histogram::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        glut_screen->scene()->background()->setColor( ::DefaultBackgroundColor );

        GLint vp[4]; glGetIntegerv( GL_VIEWPORT, vp );
        const GLint left = vp[0];
        const GLint bottom = vp[1];
        const GLint right = vp[2];
        const GLint top = vp[3];
        const float front = 0.0f;
        const float back = 2000.0f;

        glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
        {
            glDisable( GL_LIGHTING );
            glDisable( GL_DEPTH_TEST );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

            // Set 2D view.
            glMatrixMode( GL_MODELVIEW );  glPushMatrix(); glLoadIdentity();
            glMatrixMode( GL_PROJECTION ); glPushMatrix(); glLoadIdentity();
            glOrtho( left, right, bottom, top, front, back );
            {
                this->draw_histogram_texture();
            }
            glPopMatrix();
            glMatrixMode( GL_MODELVIEW );
            glPopMatrix();
        }
        glPopAttrib();
    }

private:
    void draw_histogram_texture()
    {
        const GLfloat x = 0;
        const GLfloat y = 0;
        const GLfloat width = static_cast<GLfloat>( screen()->width() );
        const GLfloat height = static_cast<GLfloat>( screen()->height() );

        glEnable( GL_TEXTURE_2D );
        m_parameters->histogram_texture.bind();
        {
            glBegin( GL_QUADS );
            {
                glTexCoord2f( 0.0f, 0.0f ); glVertex2f( x,         y );
                glTexCoord2f( 1.0f, 0.0f ); glVertex2f( x + width, y );
                glTexCoord2f( 1.0f, 1.0f ); glVertex2f( x + width, y + height );
                glTexCoord2f( 0.0f, 1.0f ); glVertex2f( x,         y + height );
            }
            glEnd();
        }
        m_parameters->histogram_texture.unbind();
    }
};

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 */
/*===========================================================================*/
class MousePressEvent : public kvs::MousePressEventListener
{
private:
    kvsview::Histogram::Parameters* m_parameters;

public:
    MousePressEvent( kvsview::Histogram::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update( kvs::MouseEvent* event )
    {
        if ( event->button() == kvs::MouseButton::Left )
        {
            m_parameters->mouse.set( event->x(), event->y() );
        }
    }
};

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 */
/*===========================================================================*/
class MouseMoveEvent : public kvs::MouseMoveEventListener
{
private:
    kvsview::Histogram::Parameters* m_parameters;

public:
    MouseMoveEvent( kvsview::Histogram::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update( kvs::MouseEvent* event )
    {
        const kvs::Vector2i diff = kvs::Vector2i( event->x(), event->y() ) - m_parameters->mouse;
        m_parameters->mouse.set( event->x(), event->y() );
        m_parameters->bias_parameter -= diff.y() * 0.005f;
        m_parameters->bias_parameter = kvs::Math::Clamp( m_parameters->bias_parameter, 0.0f, 0.99999f );
        m_parameters->updateHistogramTexture();

        screen()->redraw();
    }
};

/*===========================================================================*/
/**
 *  @brief  Key press event.
 */
/*===========================================================================*/
class KeyPressEvent : public kvs::KeyPressEventListener
{
private:
    kvsview::Histogram::Parameters* m_parameters;

public:
    KeyPressEvent( kvsview::Histogram::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update( kvs::KeyEvent* event )
    {
        if ( event->key() == kvs::Key::Home )
        {
            m_parameters->bias_parameter = 0.5f;
            m_parameters->updateHistogramTexture();
            screen()->redraw();
        }
    }
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a transfer function renderer.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv, kvsview::Histogram::CommandName )
{
    // Parameters for the transfer function renderer class.
    addOption( kvsview::Histogram::CommandName, kvsview::Histogram::Description, 0 );
    addOption( "b", "Bias parameter in [0,1]. (defalt: 0.5)", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns the bias parameter.
 *  @return bias parameter
 */
/*===========================================================================*/
const float Argument::biasParameter( void )
{
    if ( this->hasOption("b") ) return( kvs::Math::Clamp( this->optionValue<float>("b"), 0.0f, 1.0f ) );
    else return( ::DefaultBiasParameter );
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
int Main::exec( int argc, char** argv )
{
    // Setup GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Commandline arguments.
    kvsview::Histogram::Argument arg( argc, argv );
    if ( !arg.parse() ) exit( EXIT_FAILURE );
    m_input_name = arg.value<std::string>();

    // Parameters.
    kvsview::Histogram::Parameters params( arg );
    if ( !params.createFrequencyTable( m_input_name ) ) exit( EXIT_FAILURE );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        std::cout << std::endl;
        std::cout << "HISTOGRAM INFORMATION" << std::endl;
        std::cout << "    min range: " << params.frequency_table.minRange() << std::endl;
        std::cout << "    max range: " << params.frequency_table.maxRange() << std::endl;
        std::cout << "    number of bins: " << params.frequency_table.numberOfBins() << std::endl;
        std::cout << "    max count: " << params.frequency_table.maxCount() << std::endl;
    }

    // Viewer events.
    kvsview::Histogram::InitializeEvent initialize_event( &params );
    kvsview::Histogram::PaintEvent paint_event( &params );
    kvsview::Histogram::MousePressEvent mouse_press_event( &params );
    kvsview::Histogram::MouseMoveEvent mouse_move_event( &params );
    kvsview::Histogram::KeyPressEvent key_press_event( &params );

    // Rendering screen.
    kvs::glut::Screen screen( &app );
    screen.addEvent( &initialize_event );
    screen.addEvent( &paint_event );
    screen.setEvent( &mouse_press_event );
    screen.setEvent( &mouse_move_event );
    screen.addEvent( &key_press_event );
    screen.setGeometry( 0, 0, ::DefaultScreenWidth, ::DefaultScreenHeight );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::Histogram::CommandName );
    screen.show();

    return ( arg.clear(), app.run() );
}

} // end of namespace Histogram

} // end of namespace kvsview
