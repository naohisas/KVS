/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::RGBFormulae class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/ColorMapBar>
#include <kvs/RGBFormulae>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    const size_t margin = 10;
    const size_t resolution = 256;
    const size_t width = 200;
    const size_t height = 50;

    const size_t nrows = 9;
    const size_t ncols = 1;
    const size_t screen_width = ( width + margin * 2 ) * ncols;
    const size_t screen_height = height * nrows + margin + 5;

    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::RGBFormulae" );
    screen.setSize( screen_width, screen_height );
    screen.create();

    // PM3D
    kvs::ColorMapBar pm3d_cmap( &screen );
    pm3d_cmap.setCaption( "PM3D" );
    pm3d_cmap.setColorMap( kvs::RGBFormulae::PM3D( resolution ) );
    pm3d_cmap.setColorBarWidth( width );
    pm3d_cmap.setHeight( height );
    pm3d_cmap.setMargin( margin );
    pm3d_cmap.hideRange();
    pm3d_cmap.anchorToTopLeft();
    pm3d_cmap.show();

    // Green-Red-Violet
    kvs::ColorMapBar green_red_violet_cmap( &screen );
    green_red_violet_cmap.setCaption( "GreenRedViolet" );
    green_red_violet_cmap.setColorMap( kvs::RGBFormulae::GreenRedViolet( resolution ) );
    green_red_violet_cmap.setColorBarWidth( width );
    green_red_violet_cmap.setHeight( height );
    green_red_violet_cmap.setMargin( margin );
    green_red_violet_cmap.hideRange();
    green_red_violet_cmap.anchorToBottom( &pm3d_cmap );
    green_red_violet_cmap.show();

    // Ocean
    kvs::ColorMapBar ocean_cmap( &screen );
    ocean_cmap.setCaption( "Ocean" );
    ocean_cmap.setColorMap( kvs::RGBFormulae::Ocean( resolution ) );
    ocean_cmap.setColorBarWidth( width );
    ocean_cmap.setHeight( height );
    ocean_cmap.setMargin( margin );
    ocean_cmap.hideRange();
    ocean_cmap.anchorToBottom( &green_red_violet_cmap );
    ocean_cmap.show();

    // Hot
    kvs::ColorMapBar hot_cmap( &screen );
    hot_cmap.setCaption( "Hot" );
    hot_cmap.setColorMap( kvs::RGBFormulae::Hot( resolution ) );
    hot_cmap.setColorBarWidth( width );
    hot_cmap.setHeight( height );
    hot_cmap.setMargin( margin );
    hot_cmap.hideRange();
    hot_cmap.anchorToBottom( &ocean_cmap );
    hot_cmap.show();

    // Color printable
    kvs::ColorMapBar color_printable_cmap( &screen );
    color_printable_cmap.setCaption( "ColorPrintable" );
    color_printable_cmap.setColorMap( kvs::RGBFormulae::ColorPrintable( resolution ) );
    color_printable_cmap.setColorBarWidth( width );
    color_printable_cmap.setHeight( height );
    color_printable_cmap.setMargin( margin );
    color_printable_cmap.hideRange();
    color_printable_cmap.anchorToBottom( &hot_cmap );
    color_printable_cmap.show();

    // Rainbow
    kvs::ColorMapBar rainbow_cmap( &screen );
    rainbow_cmap.setCaption( "Rainbow" );
    rainbow_cmap.setColorMap( kvs::RGBFormulae::Rainbow( resolution ) );
    rainbow_cmap.setColorBarWidth( width );
    rainbow_cmap.setHeight( height );
    rainbow_cmap.setMargin( margin );
    rainbow_cmap.hideRange();
    rainbow_cmap.anchorToBottom( &color_printable_cmap );
    rainbow_cmap.show();

    // AFMHot
    kvs::ColorMapBar afm_hot_cmap( &screen );
    afm_hot_cmap.setCaption( "AFMHot" );
    afm_hot_cmap.setColorMap( kvs::RGBFormulae::AFMHot( resolution ) );
    afm_hot_cmap.setColorBarWidth( width );
    afm_hot_cmap.setHeight( height );
    afm_hot_cmap.setMargin( margin );
    afm_hot_cmap.hideRange();
    afm_hot_cmap.anchorToBottom( &rainbow_cmap );
    afm_hot_cmap.show();

    // Jet
    kvs::ColorMapBar jet_cmap( &screen );
    jet_cmap.setCaption( "Jet" );
    jet_cmap.setColorMap( kvs::RGBFormulae::Jet( resolution ) );
    jet_cmap.setColorBarWidth( width );
    jet_cmap.setHeight( height );
    jet_cmap.setMargin( margin );
    jet_cmap.hideRange();
    jet_cmap.anchorToBottom( &afm_hot_cmap );
    jet_cmap.show();

    // Bone
    kvs::ColorMapBar bone_cmap( &screen );
    bone_cmap.setCaption( "Bone" );
    bone_cmap.setColorMap( kvs::RGBFormulae::Bone( resolution ) );
    bone_cmap.setColorBarWidth( width );
    bone_cmap.setHeight( height );
    bone_cmap.setMargin( margin );
    bone_cmap.hideRange();
    bone_cmap.anchorToBottom( &jet_cmap );
    bone_cmap.show();

    return app.run();
}

