/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ColorMap (diverging colormap) class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
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
    const size_t margin = 10;
    const size_t resolution = 256;
    const size_t width = 200;
    const size_t height = 50;

    const size_t nrows = 7;
    const size_t ncols = 2;
    const size_t screen_width = ( width + margin * 2 ) * ncols;
    const size_t screen_height = height * nrows + margin + 5;
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ColorMap (Diverging Colormap)" );
    screen.setSize( screen_width, screen_height );
    screen.create();

    // Cool-Warm
    kvs::ColorMapBar cool_warm_cmap( &screen );
    cool_warm_cmap.setCaption( "kvs::ColorMap::CoolWarm" );
    cool_warm_cmap.setColorMap( kvs::ColorMap::CoolWarm( resolution ) );
    cool_warm_cmap.setColorBarWidth( width );
    cool_warm_cmap.setHeight( height );
    cool_warm_cmap.setMargin( margin );
    cool_warm_cmap.hideRange();
    cool_warm_cmap.anchorToTopLeft();
    cool_warm_cmap.show();

    // BrBG: brown, (white), blue-green
    kvs::ColorMapBar brbg_cmap( &screen );
    brbg_cmap.setCaption( "kvs::ColorMap::BrewerBrBG" );
    brbg_cmap.setColorMap( kvs::ColorMap::BrewerBrBG( resolution ) );
    brbg_cmap.setColorBarWidth( width );
    brbg_cmap.setHeight( height );
    brbg_cmap.setMargin( margin );
    brbg_cmap.hideRange();
    brbg_cmap.anchorToBottom( &cool_warm_cmap );
    brbg_cmap.show();

    // PiYG: pink, (white), yello-green
    kvs::ColorMapBar piyg_cmap( &screen );
    piyg_cmap.setCaption( "kvs::ColorMap::BrewerPiYG" );
    piyg_cmap.setColorMap( kvs::ColorMap::BrewerPiYG( resolution ) );
    piyg_cmap.setColorBarWidth( width );
    piyg_cmap.setHeight( height );
    piyg_cmap.setMargin( margin );
    piyg_cmap.hideRange();
    piyg_cmap.anchorToBottom( &brbg_cmap );
    piyg_cmap.show();

    // PRGn: purple, (white), green
    kvs::ColorMapBar prgn_cmap( &screen );
    prgn_cmap.setCaption( "kvs::ColorMap::BrewerPRGn" );
    prgn_cmap.setColorMap( kvs::ColorMap::BrewerPRGn( resolution ) );
    prgn_cmap.setColorBarWidth( width );
    prgn_cmap.setHeight( height );
    prgn_cmap.setMargin( margin );
    prgn_cmap.hideRange();
    prgn_cmap.anchorToBottom( &piyg_cmap );
    prgn_cmap.show();

    // PuOr: orange, (white), purple
    kvs::ColorMapBar puor_cmap( &screen );
    puor_cmap.setCaption( "kvs::ColorMap::BrewerPuOr" );
    puor_cmap.setColorMap( kvs::ColorMap::BrewerPuOr( resolution ) );
    puor_cmap.setColorBarWidth( width );
    puor_cmap.setHeight( height );
    puor_cmap.setMargin( margin );
    puor_cmap.hideRange();
    puor_cmap.anchorToBottom( &prgn_cmap );
    puor_cmap.show();

    // RdBu: red, (white), blue
    kvs::ColorMapBar rdbu_cmap( &screen );
    rdbu_cmap.setCaption( "kvs::ColorMap::BrewerRdBu" );
    rdbu_cmap.setColorMap( kvs::ColorMap::BrewerRdBu( resolution ) );
    rdbu_cmap.setColorBarWidth( width );
    rdbu_cmap.setHeight( height );
    rdbu_cmap.setMargin( margin );
    rdbu_cmap.hideRange();
    rdbu_cmap.anchorToBottom( &puor_cmap );
    rdbu_cmap.show();

    // RdGy: red, (white), gray
    kvs::ColorMapBar rdgy_cmap( &screen );
    rdgy_cmap.setCaption( "kvs::ColorMap::BrewerRdGy" );
    rdgy_cmap.setColorMap( kvs::ColorMap::BrewerRdGy( resolution ) );
    rdgy_cmap.setColorBarWidth( width );
    rdgy_cmap.setHeight( height );
    rdgy_cmap.setMargin( margin );
    rdgy_cmap.hideRange();
    rdgy_cmap.anchorToBottom( &rdbu_cmap );
    rdgy_cmap.show();

    // RdYlBu: red, yello, blue
    kvs::ColorMapBar rdylbu_cmap( &screen );
    rdylbu_cmap.setCaption( "kvs::ColorMap::BrewerRdYlBu" );
    rdylbu_cmap.setColorMap( kvs::ColorMap::BrewerRdYlBu( resolution ) );
    rdylbu_cmap.setColorBarWidth( width );
    rdylbu_cmap.setHeight( height );
    rdylbu_cmap.setMargin( margin );
    rdylbu_cmap.hideRange();
    rdylbu_cmap.anchorToRight( &cool_warm_cmap );
    rdylbu_cmap.show();

    // RdYlGn: red, yello, green
    kvs::ColorMapBar rdylgn_cmap( &screen );
    rdylgn_cmap.setCaption( "kvs::ColorMap::BrewerRdYlGn" );
    rdylgn_cmap.setColorMap( kvs::ColorMap::BrewerRdYlGn( resolution ) );
    rdylgn_cmap.setColorBarWidth( width );
    rdylgn_cmap.setHeight( height );
    rdylgn_cmap.setMargin( margin );
    rdylgn_cmap.hideRange();
    rdylgn_cmap.anchorToBottom( &rdylbu_cmap );
    rdylgn_cmap.show();

    // Spectral: red, orange, yello, green, blue
    kvs::ColorMapBar spectral_cmap( &screen );
    spectral_cmap.setCaption( "kvs::ColorMap::BrewerSpectral" );
    spectral_cmap.setColorMap( kvs::ColorMap::BrewerSpectral( resolution ) );
    spectral_cmap.setColorBarWidth( width );
    spectral_cmap.setHeight( height );
    spectral_cmap.setMargin( margin );
    spectral_cmap.hideRange();
    spectral_cmap.anchorToBottom( &rdylgn_cmap );
    spectral_cmap.show();

    // Berlin
    kvs::ColorMapBar berlin_cmap( &screen );
    berlin_cmap.setCaption( "kvs::ColorMap::Berlin" );
    berlin_cmap.setColorMap( kvs::ColorMap::Berlin( resolution ) );
    berlin_cmap.setColorBarWidth( width );
    berlin_cmap.setHeight( height );
    berlin_cmap.setMargin( margin );
    berlin_cmap.hideRange();
    berlin_cmap.anchorToBottom( &spectral_cmap );
    berlin_cmap.show();

    // Managua
    kvs::ColorMapBar managua_cmap( &screen );
    managua_cmap.setCaption( "kvs::ColorMap::Managua" );
    managua_cmap.setColorMap( kvs::ColorMap::Managua( resolution ) );
    managua_cmap.setColorBarWidth( width );
    managua_cmap.setHeight( height );
    managua_cmap.setMargin( margin );
    managua_cmap.hideRange();
    managua_cmap.anchorToBottom( &berlin_cmap );
    managua_cmap.show();

    // Vanimo
    kvs::ColorMapBar vanimo_cmap( &screen );
    vanimo_cmap.setCaption( "kvs::ColorMap::Vanimo" );
    vanimo_cmap.setColorMap( kvs::ColorMap::Vanimo( resolution ) );
    vanimo_cmap.setColorBarWidth( width );
    vanimo_cmap.setHeight( height );
    vanimo_cmap.setMargin( margin );
    vanimo_cmap.hideRange();
    vanimo_cmap.anchorToBottom( &managua_cmap );
    vanimo_cmap.show();

    // Original
    const auto c1 = kvs::RGBColor(  35, 127, 146 );
    const auto c2 = kvs::RGBColor( 207,  85,  62 );
    kvs::ColorMapBar original_cmap( &screen );
    original_cmap.setCaption( "kvs::ColorMap::Diverging" );
    original_cmap.setColorMap( kvs::ColorMap::Diverging( c1, c2, resolution ) );
    original_cmap.setColorBarWidth( width );
    original_cmap.setHeight( height );
    original_cmap.setMargin( margin );
    original_cmap.hideRange();
    original_cmap.anchorToBottom( &vanimo_cmap );
    original_cmap.show();

    return app.run();
}
