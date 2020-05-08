/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::RGBFormulae class.
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ColorMapBar>
#include <kvs/RGBFormulae>
#include <kvs/Label>


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
    kvs::glut::Application app( argc, argv );

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::RGBFormulae" );
    screen.setBackgroundColor( kvs::RGBColor::White() );
    screen.setSize( 256, 590 );
    screen.show();

    const kvs::Font font( kvs::Font::Sans, kvs::Font::Bold, 20 );
    const size_t h = kvs::FontMetrics( font, screen.paintDevice() ).height();

    // PM3D
    kvs::Label pm3d_label( &screen );
    pm3d_label.setText( "PM3D" );
    pm3d_label.setFont( font );
    pm3d_label.setPosition( 10, 0 );
    pm3d_label.show();

    kvs::ColorMapBar pm3d_cmap( &screen );
    pm3d_cmap.setColorMap( kvs::RGBFormulae::PM3D( 256 ) );
    pm3d_cmap.setPosition( pm3d_label.x() - 10, pm3d_label.y() + h - 5 );
    pm3d_cmap.setSize( 256, 50 );
    pm3d_cmap.hideRange();
    pm3d_cmap.show();

    // Green-Red-Violet
    kvs::Label green_red_violet_label( &screen );
    green_red_violet_label.setText( "GreenRedViolet" );
    green_red_violet_label.setFont( font );
    green_red_violet_label.setPosition( 10, pm3d_cmap.y() + pm3d_cmap.height() );
    green_red_violet_label.show();

    kvs::ColorMapBar green_red_violet_cmap( &screen );
    green_red_violet_cmap.setColorMap( kvs::RGBFormulae::GreenRedViolet( 256 ) );
    green_red_violet_cmap.setPosition( green_red_violet_label.x() - 10, green_red_violet_label.y() + h - 5 );
    green_red_violet_cmap.setSize( 256, 50 );
    green_red_violet_cmap.hideRange();
    green_red_violet_cmap.show();

    // Ocean
    kvs::Label ocean_label( &screen );
    ocean_label.setText( "Ocean" );
    ocean_label.setFont( font );
    ocean_label.setPosition( 10, green_red_violet_cmap.y() + green_red_violet_cmap.height() );
    ocean_label.show();

    kvs::ColorMapBar ocean_cmap( &screen );
    ocean_cmap.setColorMap( kvs::RGBFormulae::Ocean( 256 ) );
    ocean_cmap.setPosition( ocean_label.x() - 10, ocean_label.y() + h - 5 );
    ocean_cmap.setSize( 256, 50 );
    ocean_cmap.hideRange();
    ocean_cmap.show();

    // Hot
    kvs::Label hot_label( &screen );
    hot_label.setText( "Hot" );
    hot_label.setFont( font );
    hot_label.setPosition( 10, ocean_cmap.y() + ocean_cmap.height() );
    hot_label.show();

    kvs::ColorMapBar hot_cmap( &screen );
    hot_cmap.setColorMap( kvs::RGBFormulae::Hot( 256 ) );
    hot_cmap.setPosition( hot_label.x() - 10, hot_label.y() + h - 5 );
    hot_cmap.setSize( 256, 50 );
    hot_cmap.hideRange();
    hot_cmap.show();

    // Color printable
    kvs::Label color_printable_label( &screen );
    color_printable_label.setText( "ColorPrintable" );
    color_printable_label.setFont( font );
    color_printable_label.setPosition( 10, hot_cmap.y() + hot_cmap.height() );
    color_printable_label.show();

    kvs::ColorMapBar color_printable_cmap( &screen );
    color_printable_cmap.setColorMap( kvs::RGBFormulae::ColorPrintable( 256 ) );
    color_printable_cmap.setPosition( color_printable_label.x() - 10, color_printable_label.y() + h - 5 );
    color_printable_cmap.setSize( 256, 50 );
    color_printable_cmap.hideRange();
    color_printable_cmap.show();

    // Rainbow
    kvs::Label rainbow_label( &screen );
    rainbow_label.setText( "Rainbow" );
    rainbow_label.setFont( font );
    rainbow_label.setPosition( 10, color_printable_cmap.y() + color_printable_cmap.height() );
    rainbow_label.show();

    kvs::ColorMapBar rainbow_cmap( &screen );
    rainbow_cmap.setColorMap( kvs::RGBFormulae::Rainbow( 256 ) );
    rainbow_cmap.setPosition( rainbow_label.x() - 10, rainbow_label.y() + h - 5 );
    rainbow_cmap.setSize( 256, 50 );
    rainbow_cmap.hideRange();
    rainbow_cmap.show();

    // AFMHot
    kvs::Label afm_hot_label( &screen );
    afm_hot_label.setText( "AFMHot" );
    afm_hot_label.setFont( font );
    afm_hot_label.setPosition( 10, rainbow_cmap.y() + rainbow_cmap.height() );
    afm_hot_label.show();

    kvs::ColorMapBar afm_hot_cmap( &screen );
    afm_hot_cmap.setColorMap( kvs::RGBFormulae::AFMHot( 256 ) );
    afm_hot_cmap.setPosition( afm_hot_label.x() - 10, afm_hot_label.y() + h - 5 );
    afm_hot_cmap.setSize( 256, 50 );
    afm_hot_cmap.hideRange();
    afm_hot_cmap.show();

    // Jet
    kvs::Label jet_label( &screen );
    jet_label.setText( "Jet" );
    jet_label.setFont( font );
    jet_label.setPosition( 10, afm_hot_cmap.y() + afm_hot_cmap.height() );
    jet_label.show();

    kvs::ColorMapBar jet_cmap( &screen );
    jet_cmap.setColorMap( kvs::RGBFormulae::Jet( 256 ) );
    jet_cmap.setPosition( jet_label.x() - 10, jet_label.y() + h - 5 );
    jet_cmap.setSize( 256, 50 );
    jet_cmap.hideRange();
    jet_cmap.show();

    // Bone
    kvs::Label bone_label( &screen );
    bone_label.setText( "Bone" );
    bone_label.setFont( font );
    bone_label.setPosition( 10, jet_cmap.y() + jet_cmap.height() );
    bone_label.show();

    kvs::ColorMapBar bone_cmap( &screen );
    bone_cmap.setColorMap( kvs::RGBFormulae::Bone( 256 ) );
    bone_cmap.setPosition( bone_label.x() - 10, bone_label.y() + h - 5 );
    bone_cmap.setSize( 256, 50 );
    bone_cmap.hideRange();
    bone_cmap.show();

    return app.run();
}

