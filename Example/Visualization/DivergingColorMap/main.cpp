/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ColorMap (diverging colormap) class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/Label>
#include <kvs/ColorMapBar>
#include <kvs/ColorMap>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::ColorMap" );
    screen.setBackgroundColor( kvs::RGBColor::White() );
    screen.setSize( 256, 655 );
    screen.show();

    const kvs::Font font( kvs::Font::Sans, kvs::Font::Bold, 20 );
    const size_t h = kvs::FontMetrics( font, screen.paintDevice() ).height();

    // Cool-Warm
    kvs::Label cool_warm_label( &screen );
    cool_warm_label.setText( "CoolWarm" );
    cool_warm_label.setFont( font );
    cool_warm_label.setPosition( 10, 0 );
    cool_warm_label.show();

    kvs::ColorMapBar cool_warm_cmap( &screen );
    cool_warm_cmap.setColorMap( kvs::ColorMap::CoolWarm( 256 ) );
    cool_warm_cmap.setPosition( cool_warm_label.x() - 10, cool_warm_label.y() + h - 5 );
    cool_warm_cmap.setSize( 256, 50 );
    cool_warm_cmap.hideRange();
    cool_warm_cmap.show();

    // BrBG: brown, (white), blue-green
    kvs::Label brbg_label( &screen );
    brbg_label.setText( "BrewerBrBG" );
    brbg_label.setFont( font );
    brbg_label.setPosition( 10, cool_warm_cmap.y() + cool_warm_cmap.height() );
    brbg_label.show();

    kvs::ColorMapBar brbg_cmap( &screen );
    brbg_cmap.setColorMap( kvs::ColorMap::BrewerBrBG( 256 ) );
    brbg_cmap.setPosition( brbg_label.x() - 10, brbg_label.y() + h - 5 );
    brbg_cmap.setSize( 256, 50 );
    brbg_cmap.hideRange();
    brbg_cmap.show();

    // PiYG: pink, (white), yello-green
    kvs::Label piyg_label( &screen );
    piyg_label.setText( "BrewerPiYG" );
    piyg_label.setFont( font );
    piyg_label.setPosition( 10, brbg_cmap.y() + brbg_cmap.height() );
    piyg_label.show();

    kvs::ColorMapBar piyg_cmap( &screen );
    piyg_cmap.setColorMap( kvs::ColorMap::BrewerPiYG( 256 ) );
    piyg_cmap.setPosition( piyg_label.x() - 10, piyg_label.y() + h - 5 );
    piyg_cmap.setSize( 256, 50 );
    piyg_cmap.hideRange();
    piyg_cmap.show();

    // PRGn: purple, (white), green
    kvs::Label prgn_label( &screen );
    prgn_label.setText( "BrewerPRGn" );
    prgn_label.setFont( font );
    prgn_label.setPosition( 10, piyg_cmap.y() + piyg_cmap.height() );
    prgn_label.show();

    kvs::ColorMapBar prgn_cmap( &screen );
    prgn_cmap.setColorMap( kvs::ColorMap::BrewerPRGn( 256 ) );
    prgn_cmap.setPosition( prgn_label.x() - 10, prgn_label.y() + h - 5 );
    prgn_cmap.setSize( 256, 50 );
    prgn_cmap.hideRange();
    prgn_cmap.show();

    // PuOr: orange, (white), purple
    kvs::Label puor_label( &screen );
    puor_label.setText( "BrewerPuOr" );
    puor_label.setFont( font );
    puor_label.setPosition( 10, prgn_cmap.y() + prgn_cmap.height() );
    puor_label.show();

    kvs::ColorMapBar puor_cmap( &screen );
    puor_cmap.setColorMap( kvs::ColorMap::BrewerPuOr( 256 ) );
    puor_cmap.setPosition( puor_label.x() - 10, puor_label.y() + h - 5 );
    puor_cmap.setSize( 256, 50 );
    puor_cmap.hideRange();
    puor_cmap.show();

    // RdBu: red, (white), blue
    kvs::Label rdbu_label( &screen );
    rdbu_label.setText( "BrewerRdBu" );
    rdbu_label.setFont( font );
    rdbu_label.setPosition( 10, puor_cmap.y() + puor_cmap.height() );
    rdbu_label.show();

    kvs::ColorMapBar rdbu_cmap( &screen );
    rdbu_cmap.setColorMap( kvs::ColorMap::BrewerRdBu( 256 ) );
    rdbu_cmap.setPosition( rdbu_label.x() - 10, rdbu_label.y() + h - 5 );
    rdbu_cmap.setSize( 256, 50 );
    rdbu_cmap.hideRange();
    rdbu_cmap.show();

    // RdGy: red, (white), gray
    kvs::Label rdgy_label( &screen );
    rdgy_label.setText( "BrewerRdGy" );
    rdgy_label.setFont( font );
    rdgy_label.setPosition( 10, rdbu_cmap.y() + rdbu_cmap.height() );
    rdgy_label.show();

    kvs::ColorMapBar rdgy_cmap( &screen );
    rdgy_cmap.setColorMap( kvs::ColorMap::BrewerRdGy( 256 ) );
    rdgy_cmap.setPosition( rdgy_label.x() - 10, rdgy_label.y() + h - 5 );
    rdgy_cmap.setSize( 256, 50 );
    rdgy_cmap.hideRange();
    rdgy_cmap.show();

    // RdYlBu: red, yello, blue
    kvs::Label rdylbu_label( &screen );
    rdylbu_label.setText( "BrewerRdYlBu" );
    rdylbu_label.setFont( font );
    rdylbu_label.setPosition( 10, rdgy_cmap.y() + rdgy_cmap.height() );
    rdylbu_label.show();

    kvs::ColorMapBar rdylbu_cmap( &screen );
    rdylbu_cmap.setColorMap( kvs::ColorMap::BrewerRdYlBu( 256 ) );
    rdylbu_cmap.setPosition( rdylbu_label.x() - 10, rdylbu_label.y() + h - 5 );
    rdylbu_cmap.setSize( 256, 50 );
    rdylbu_cmap.hideRange();
    rdylbu_cmap.show();

    // RdYlGn: red, yello, green
    kvs::Label rdylgn_label( &screen );
    rdylgn_label.setText( "BrewerRdYlGn" );
    rdylgn_label.setFont( font );
    rdylgn_label.setPosition( 10, rdylbu_cmap.y() + rdylbu_cmap.height() );
    rdylgn_label.show();

    kvs::ColorMapBar rdylgn_cmap( &screen );
    rdylgn_cmap.setColorMap( kvs::ColorMap::BrewerRdYlGn( 256 ) );
    rdylgn_cmap.setPosition( rdylgn_label.x() - 10, rdylgn_label.y() + h - 5 );
    rdylgn_cmap.setSize( 256, 50 );
    rdylgn_cmap.hideRange();
    rdylgn_cmap.show();

    // Spectral: red, orange, yello, green, blue
    kvs::Label spectral_label( &screen );
    spectral_label.setText( "BrewerSpectral" );
    spectral_label.setFont( font );
    spectral_label.setPosition( 10, rdylgn_cmap.y() + rdylgn_cmap.height() );
    spectral_label.show();

    kvs::ColorMapBar spectral_cmap( &screen );
    spectral_cmap.setColorMap( kvs::ColorMap::BrewerSpectral( 256 ) );
    spectral_cmap.setPosition( spectral_label.x() - 10, spectral_label.y() + h - 5 );
    spectral_cmap.setSize( 256, 50 );
    spectral_cmap.hideRange();
    spectral_cmap.show();

    return app.run();
}
