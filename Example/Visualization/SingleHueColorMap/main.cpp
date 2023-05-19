/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ColorMap (sequential colormap) class.
 *  @author Tomomasa Uchida
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
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ColorMap (ColorBrewer's Sequential Colormap - Single-Hue)" );
    screen.setSize( 256, 400 );
    screen.create();

    const kvs::Font font( kvs::Font::Sans, kvs::Font::Bold, 20 );
    const size_t h = kvs::FontMetrics( font, screen.paintDevice() ).height();
    const size_t r = 256;

    // Greys
    kvs::Label greys_label( &screen );
    greys_label.setText( "Greys" );
    greys_label.setFont( font );
    greys_label.setPosition( 10, 0 );
    greys_label.show();

    kvs::ColorMapBar greys_cmap( &screen );
    greys_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueGreys( r ) );
    greys_cmap.setPosition( greys_label.x() - 10, greys_label.y() + h - 5 );
    greys_cmap.setSize( 256, 50 );
    greys_cmap.hideRange();
    greys_cmap.show();

    // Greens
    kvs::Label greens_label( &screen );
    greens_label.setText( "Greens" );
    greens_label.setFont( font );
    greens_label.setPosition( 10, greys_cmap.y() + greys_cmap.height() );
    greens_label.show();

    kvs::ColorMapBar greens_cmap( &screen );
    greens_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueGreens( r ) );
    greens_cmap.setPosition( greens_label.x() - 10, greens_label.y() + h - 5 );
    greens_cmap.setSize( 256, 50 );
    greens_cmap.hideRange();
    greens_cmap.show();

    // Blues
    kvs::Label blues_label( &screen );
    blues_label.setText( "Blues" );
    blues_label.setFont( font );
    blues_label.setPosition( 10, greens_cmap.y() + greens_cmap.height() );
    blues_label.show();

    kvs::ColorMapBar blues_cmap( &screen );
    blues_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueBlues( r ) );
    blues_cmap.setPosition( blues_label.x() - 10, blues_label.y() + h - 5 );
    blues_cmap.setSize( 256, 50 );
    blues_cmap.hideRange();
    blues_cmap.show();

    // Reds
    kvs::Label reds_label( &screen );
    reds_label.setText( "Reds" );
    reds_label.setFont( font );
    reds_label.setPosition( 10, blues_cmap.y() + blues_cmap.height() );
    reds_label.show();

    kvs::ColorMapBar reds_cmap( &screen );
    reds_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueReds( r ) );
    reds_cmap.setPosition( reds_label.x() - 10, reds_label.y() + h - 5 );
    reds_cmap.setSize( 256, 50 );
    reds_cmap.hideRange();
    reds_cmap.show();

    // Purples
    kvs::Label purples_label( &screen );
    purples_label.setText( "Purples" );
    purples_label.setFont( font );
    purples_label.setPosition( 10, reds_cmap.y() + reds_cmap.height() );
    purples_label.show();

    kvs::ColorMapBar purples_cmap( &screen );
    purples_cmap.setColorMap( kvs::ColorMap::BrewerSingleHuePurples( r ) );
    purples_cmap.setPosition( purples_label.x() - 10, purples_label.y() + h - 5 );
    purples_cmap.setSize( 256, 50 );
    purples_cmap.hideRange();
    purples_cmap.show();

    // Oranges
    kvs::Label oranges_label( &screen );
    oranges_label.setText( "Oranges" );
    oranges_label.setFont( font );
    oranges_label.setPosition( 10, purples_cmap.y() + purples_cmap.height() );
    oranges_label.show();

    kvs::ColorMapBar oranges_cmap( &screen );
    oranges_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueOranges( r ) );
    oranges_cmap.setPosition( oranges_label.x() - 10, oranges_label.y() + h - 5 );
    oranges_cmap.setSize( 256, 50 );
    oranges_cmap.hideRange();
    oranges_cmap.show();

    return app.run();
}
