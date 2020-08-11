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
    screen.setTitle( "kvs::ColorMap (Sequential Colormap)" );
    screen.setSize( 256, 330 );
    screen.create();

    const kvs::Font font( kvs::Font::Sans, kvs::Font::Bold, 20 );
    const size_t h = kvs::FontMetrics( font, screen.paintDevice() ).height();

    // Viridis
    kvs::Label viridis_label( &screen );
    viridis_label.setText( "Viridis" );
    viridis_label.setFont( font );
    viridis_label.setPosition( 10, 0 );
    viridis_label.show();

    kvs::ColorMapBar viridis_cmap( &screen );
    viridis_cmap.setColorMap( kvs::ColorMap::Viridis( 256 ) );
    viridis_cmap.setPosition( viridis_label.x() - 10, viridis_label.y() + h - 5 );
    viridis_cmap.setSize( 256, 50 );
    viridis_cmap.hideRange();
    viridis_cmap.show();

    // Plasma
    kvs::Label plasma_label( &screen );
    plasma_label.setText( "Plasma" );
    plasma_label.setFont( font );
    plasma_label.setPosition( 10, viridis_cmap.y() + viridis_cmap.height() );
    plasma_label.show();

    kvs::ColorMapBar plasma_cmap( &screen );
    plasma_cmap.setColorMap( kvs::ColorMap::Plasma( 256 ) );
    plasma_cmap.setPosition( plasma_label.x() - 10, plasma_label.y() + h - 5 );
    plasma_cmap.setSize( 256, 50 );
    plasma_cmap.hideRange();
    plasma_cmap.show();

    // Inferno
    kvs::Label inferno_label( &screen );
    inferno_label.setText( "Inferno" );
    inferno_label.setFont( font );
    inferno_label.setPosition( 10, plasma_cmap.y() + plasma_cmap.height() );
    inferno_label.show();

    kvs::ColorMapBar inferno_cmap( &screen );
    inferno_cmap.setColorMap( kvs::ColorMap::Inferno( 256 ) );
    inferno_cmap.setPosition( inferno_label.x() - 10, inferno_label.y() + h - 5 );
    inferno_cmap.setSize( 256, 50 );
    inferno_cmap.hideRange();
    inferno_cmap.show();

    // Magma
    kvs::Label magma_label( &screen );
    magma_label.setText( "Magma" );
    magma_label.setFont( font );
    magma_label.setPosition( 10, inferno_cmap.y() + inferno_cmap.height() );
    magma_label.show();

    kvs::ColorMapBar magma_cmap( &screen );
    magma_cmap.setColorMap( kvs::ColorMap::Magma( 256 ) );
    magma_cmap.setPosition( magma_label.x() - 10, magma_label.y() + h - 5 );
    magma_cmap.setSize( 256, 50 );
    magma_cmap.hideRange();
    magma_cmap.show();

    // Cividis
    kvs::Label cividis_label( &screen );
    cividis_label.setText( "Cividis" );
    cividis_label.setFont( font );
    cividis_label.setPosition( 10, magma_cmap.y() + magma_cmap.height() );
    cividis_label.show();

    kvs::ColorMapBar cividis_cmap( &screen );
    cividis_cmap.setColorMap( kvs::ColorMap::Cividis( 256 ) );
    cividis_cmap.setPosition( cividis_label.x() - 10, cividis_label.y() + h - 5 );
    cividis_cmap.setSize( 256, 50 );
    cividis_cmap.hideRange();
    cividis_cmap.show();

    return app.run();
}