/*****************************************************************************/
/**
 *  @file   TransferFunction.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunction.cpp 1551 2013-04-20 01:40:09Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TransferFunction.h"
#include <kvs/DebugNew>
#include <kvs/TransferFunction>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/OpenGL>
#include <kvs/InitializeEventListener>
#include <kvs/PaintEventListener>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Background>
#include "CommandName.h"


namespace kvsview
{

namespace TransferFunction
{

/*===========================================================================*/
/**
 *  @brief  Parameter set.
 */
/*===========================================================================*/
struct Parameters
{
    kvs::TransferFunction transfer_function; ///< transfer function
    kvs::Texture1D color_map_texture; ///< color map texture
    kvs::Texture2D checkerboard_texture; ///< checkerboard texture
    bool has_color_map_option; ///< check flag for color map
    bool has_opacity_map_option; ///< check flag for opacity map

    Parameters( Argument& arg )
    {
        has_color_map_option = arg.hasColorMapOption();
        has_opacity_map_option = arg.hasOpacityMapOption();
    }

    void initializeColorMapTexture()
    {
        const size_t nchannels  = 4; // rgba
        const size_t width = transfer_function.colorMap().resolution();
        const kvs::UInt8* color_map = transfer_function.colorMap().table().data();
        const kvs::Real32* opacity_map = transfer_function.opacityMap().table().data();

        kvs::ValueArray<GLubyte> data( width * nchannels );
        GLubyte* pdata = data.data();
        for ( size_t i = 0, i3 = 0; i < width; i++, i3 += 3 )
        {
            *(pdata++) = static_cast<GLubyte>(color_map[i3]);
            *(pdata++) = static_cast<GLubyte>(color_map[i3+1]);
            *(pdata++) = static_cast<GLubyte>(color_map[i3+2]);
            *(pdata++) = static_cast<GLubyte>(int(opacity_map[i] * 255.0f + 0.5));
        }

        color_map_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
        color_map_texture.setWrapS( GL_CLAMP_TO_EDGE );
        color_map_texture.setMinFilter( GL_LINEAR );
        color_map_texture.setMagFilter( GL_LINEAR );
        color_map_texture.create( width, data.data() );
    }

    void initializeCheckerboardTexture()
    {
        const size_t nchannels = 3;
        const int width = 32;
        const int height = 32;

        kvs::ValueArray<GLubyte> data( width * height * nchannels );
        GLubyte* pdata = data.data();
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

        checkerboard_texture.setPixelFormat( nchannels, sizeof( kvs::UInt8 ) );
        checkerboard_texture.setMinFilter( GL_NEAREST );
        checkerboard_texture.setMagFilter( GL_NEAREST );
        checkerboard_texture.setWrapS( GL_REPEAT );
        checkerboard_texture.setWrapT( GL_REPEAT );
        checkerboard_texture.create( width, height, data.data() );
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
    kvsview::TransferFunction::Parameters* m_parameters;

public:
    InitializeEvent( kvsview::TransferFunction::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update()
    {
        m_parameters->initializeColorMapTexture();
        m_parameters->initializeCheckerboardTexture();
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
    kvsview::TransferFunction::Parameters* m_parameters;

public:
    PaintEvent( kvsview::TransferFunction::Parameters* parameters ):
        m_parameters( parameters ) {}

    void update()
    {
        kvs::glut::Screen* glut_screen = static_cast<kvs::glut::Screen*>( screen() );
        glut_screen->setBackgroundColor( kvs::RGBColor::White() );

        kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
        kvs::OpenGL::Disable( GL_LIGHTING );
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Enable( GL_BLEND );

        GLint vp[4]; kvs::OpenGL::GetViewport( vp );
        const int left = vp[0];
        const int bottom = vp[1];
        const int right = vp[2];
        const int top = vp[3];

        kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
        p1.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
            p2.loadIdentity();
            {
                const float front = 0.0f;
                const float back = 2000.0f;
                kvs::OpenGL::SetOrtho( left, right, bottom, top, front, back );

                const bool has_color_map_option = m_parameters->has_color_map_option;
                const bool has_opacity_map_option = m_parameters->has_opacity_map_option;
                if ( has_color_map_option && !has_opacity_map_option )
                {
                    // Draw the color map witout opacity.
                    this->draw_color_map_texture( GL_ONE, GL_ZERO );
                }
                else if ( !has_color_map_option && has_opacity_map_option )
                {
                    // Draw the opacity map on the checkerboard.
                    this->draw_checkerboard_texture( GL_ONE, GL_ZERO );
                    this->draw_color_map_texture( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA );
                }
                else
                {
                    // Draw the color map with the opacity on the checkerboard.
                    this->draw_checkerboard_texture( GL_ONE, GL_ZERO );
                    this->draw_color_map_texture( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                }
            }
        }
    }

private:
    void draw_checkerboard_texture( const GLenum src_factor, const GLenum dst_factor )
    {
        // Since the checkerboard is background, GL_ONE and GL_ZERO are always specified
        // for src_factor and dst_factor, rescpectively.
        kvs::OpenGL::Disable( GL_TEXTURE_1D );
        kvs::OpenGL::Enable( GL_TEXTURE_2D );
        kvs::OpenGL::SetBlendFunc( src_factor, dst_factor );
        m_parameters->checkerboard_texture.bind();
        {
            const float scale = 0.6f;
            const float texture_width = ( screen()->width() / 32.0f ) * scale;
            const float texture_height = ( screen()->height() / 32.0f ) * scale;
            this->draw_texture( texture_width, texture_height );
        }
        m_parameters->checkerboard_texture.unbind();
    }

    void draw_color_map_texture( const GLenum src_factor, const GLenum dst_factor )
    {
        kvs::OpenGL::Enable( GL_TEXTURE_1D );
        kvs::OpenGL::Disable( GL_TEXTURE_2D );
        kvs::OpenGL::SetBlendFunc( src_factor, dst_factor );
        m_parameters->color_map_texture.bind();
        {
            const float texture_width = 1.0f;
            const float texture_height = 1.0f;
            this->draw_texture( texture_width, texture_height );
        }
        m_parameters->color_map_texture.unbind();
    }

    void draw_texture( const float texture_width, const float texture_height )
    {
        const GLfloat x = 0;
        const GLfloat y = 0;
        const GLfloat width = static_cast<GLfloat>(screen()->width());
        const GLfloat height = static_cast<GLfloat>(screen()->height());

        const kvs::Vec2 t0( 0.0f, 0.0f );
        const kvs::Vec2 t1( texture_width, 0.0f );
        const kvs::Vec2 t2( texture_width, texture_height );
        const kvs::Vec2 t3( 0.0f, texture_height );

        const kvs::Vec2 v0( x, y );
        const kvs::Vec2 v1( x + width, y );
        const kvs::Vec2 v2( x + width, y + height );
        const kvs::Vec2 v3( x, y + height );

        kvs::OpenGL::Begin( GL_QUADS );
        {
            kvs::OpenGL::TexCoordVertex( t0, v0 );
            kvs::OpenGL::TexCoordVertex( t1, v1 );
            kvs::OpenGL::TexCoordVertex( t2, v2 );
            kvs::OpenGL::TexCoordVertex( t3, v3 );
        }
        kvs::OpenGL::End();
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
    kvsview::Argument::Common( argc, argv, kvsview::TransferFunction::CommandName )
{
    // Parameters for the transfer function renderer class.
    addOption( kvsview::TransferFunction::CommandName, kvsview::TransferFunction::Description, 0 );
    addOption("c", "Display color map. (optional)", 0, false );
    addOption("a", "Display opacity (alpha) map. (optional)", 0, false );
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the color map option is specified or not.
 *  @return true, if the color map option is specified
 */
/*===========================================================================*/
const bool Argument::hasColorMapOption()
{
    return this->hasOption("c");
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the opacity map option is specified or not.
 *  @return true, if the opacity map option is specified
 */
/*===========================================================================*/
const bool Argument::hasOpacityMapOption()
{
    return this->hasOption("a");
}

 /*===========================================================================*/
/**
 *  @brief  Executes main process.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int Main::exec( int argc, char** argv )
{
    // Setup GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Commandline arguments.
    kvsview::TransferFunction::Argument arg( argc, argv );
    if ( !arg.parse() ) exit( EXIT_FAILURE );
    m_input_name = arg.value<std::string>();

    // Parameters.
    kvsview::TransferFunction::Parameters params( arg );
    if ( !params.transfer_function.read( m_input_name ) ) { exit( EXIT_FAILURE ); }

    // Verbose information.
    if ( arg.verboseMode() )
    {
        std::cout << "IMPORTED OBJECT" << std::endl;
        std::cout << "resolution: " << params.transfer_function.resolution() << std::endl;
    }

    // Events.
    kvsview::TransferFunction::InitializeEvent initialize_event( &params );
    kvsview::TransferFunction::PaintEvent paint_event( &params );

    // Create and show the rendering screen.
    kvs::glut::Screen screen( &app );
    screen.addEvent( &initialize_event );
    screen.addEvent( &paint_event );
    screen.setGeometry( 0, 0, 512, 100 );
    screen.setTitle( kvsview::CommandName + " - " + kvsview::TransferFunction::CommandName );
    screen.show();

    return arg.clear(), app.run();
}

} // end of namespace TransferFunction

} // end of namespace kvsview
