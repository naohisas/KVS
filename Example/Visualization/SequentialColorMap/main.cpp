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
    const size_t margin = 10;
    const size_t resolution = 256;
    const size_t width = 200;
    const size_t height = 50;

    const size_t nrows = 6;
    const size_t ncols = 4;
    const size_t screen_width = ( width + margin * 2 ) * ncols;
    const size_t screen_height = height * nrows + margin + 5;

    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ColorMap (Sequential Colormap)" );
    screen.setSize( screen_width, screen_height );
    screen.create();

    // Single hue greys
    kvs::ColorMapBar greys_cmap( &screen );
    greys_cmap.setCaption( "BrewerSingleHueGreys" );
    greys_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueGreys( resolution ) );
    greys_cmap.setColorBarWidth( width );
    greys_cmap.setHeight( height );
    greys_cmap.setMargin( margin );
    greys_cmap.hideRange();
    greys_cmap.anchorToTopLeft();
    greys_cmap.show();

    // Single hue greens
    kvs::ColorMapBar greens_cmap( &screen );
    greens_cmap.setCaption( "BrewerSingleHueGreens" );
    greens_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueGreens( resolution ) );
    greens_cmap.setColorBarWidth( width );
    greens_cmap.setHeight( height );
    greens_cmap.setMargin( margin );
    greens_cmap.hideRange();
    greens_cmap.anchorToBottom( &greys_cmap );
    greens_cmap.show();

    // Single hue blues
    kvs::ColorMapBar blues_cmap( &screen );
    blues_cmap.setCaption( "BrewerSingleHueBlues" );
    blues_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueBlues( resolution ) );
    blues_cmap.setColorBarWidth( width );
    blues_cmap.setHeight( height );
    blues_cmap.setMargin( margin );
    blues_cmap.hideRange();
    blues_cmap.anchorToBottom( &greens_cmap );
    blues_cmap.show();

    // Single hue reds
    kvs::ColorMapBar reds_cmap( &screen );
    reds_cmap.setCaption( "BrewerSingleHueReds" );
    reds_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueReds( resolution ) );
    reds_cmap.setColorBarWidth( width );
    reds_cmap.setHeight( height );
    reds_cmap.setMargin( margin );
    reds_cmap.hideRange();
    reds_cmap.anchorToBottom( &blues_cmap );
    reds_cmap.show();

    // Single hue purples
    kvs::ColorMapBar purples_cmap( &screen );
    purples_cmap.setCaption( "BrewerSingleHuePurples" );
    purples_cmap.setColorMap( kvs::ColorMap::BrewerSingleHuePurples( resolution ) );
    purples_cmap.setColorBarWidth( width );
    purples_cmap.setHeight( height );
    purples_cmap.setMargin( margin );
    purples_cmap.hideRange();
    purples_cmap.anchorToBottom( &reds_cmap );
    purples_cmap.show();

    // Single hue oranges
    kvs::ColorMapBar oranges_cmap( &screen );
    oranges_cmap.setCaption( "BrewerSingleHueOranges" );
    oranges_cmap.setColorMap( kvs::ColorMap::BrewerSingleHueOranges( resolution ) );
    oranges_cmap.setColorBarWidth( width );
    oranges_cmap.setHeight( height );
    oranges_cmap.setMargin( margin );
    oranges_cmap.hideRange();
    oranges_cmap.anchorToBottom( &purples_cmap );
    oranges_cmap.show();

    //--------------------------------------------------------------------------

    // multi-hue BuGn
    kvs::ColorMapBar bugn_cmap( &screen );
    bugn_cmap.setCaption( "BrewerMultiHueBuGn" );
    bugn_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueBuGn( resolution ) );
    bugn_cmap.setColorBarWidth( width );
    bugn_cmap.setHeight( height );
    bugn_cmap.setMargin( margin );
    bugn_cmap.hideRange();
    bugn_cmap.anchorToRight( &greys_cmap );
    bugn_cmap.show();

    // multi-hue BuPu
    kvs::ColorMapBar bupu_cmap( &screen );
    bupu_cmap.setCaption( "BrewerMultiHueBuPu" );
    bupu_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueBuPu( resolution ) );
    bupu_cmap.setColorBarWidth( width );
    bupu_cmap.setHeight( height );
    bupu_cmap.setMargin( margin );
    bupu_cmap.hideRange();
    bupu_cmap.anchorToBottom( &bugn_cmap );
    bupu_cmap.show();

    // multi-hue GnBu
    kvs::ColorMapBar gnbu_cmap( &screen );
    gnbu_cmap.setCaption( "BrewerMultiHueGnBu" );
    gnbu_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueGnBu( resolution ) );
    gnbu_cmap.setColorBarWidth( width );
    gnbu_cmap.setHeight( height );
    gnbu_cmap.setMargin( margin );
    gnbu_cmap.hideRange();
    gnbu_cmap.anchorToBottom( &bupu_cmap );
    gnbu_cmap.show();

    // multi-hue OrRd
    kvs::ColorMapBar orrd_cmap( &screen );
    orrd_cmap.setCaption( "BrewerMultiHueOrRd" );
    orrd_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueOrRd( resolution ) );
    orrd_cmap.setColorBarWidth( width );
    orrd_cmap.setHeight( height );
    orrd_cmap.setMargin( margin );
    orrd_cmap.hideRange();
    orrd_cmap.anchorToBottom( &gnbu_cmap );
    orrd_cmap.show();

    // multi-hue PuBu
    kvs::ColorMapBar pubu_cmap( &screen );
    pubu_cmap.setCaption( "BrewerMultiHuePuBu" );
    pubu_cmap.setColorMap( kvs::ColorMap::BrewerMultiHuePuBu( resolution ) );
    pubu_cmap.setColorBarWidth( width );
    pubu_cmap.setHeight( height );
    pubu_cmap.setMargin( margin );
    pubu_cmap.hideRange();
    pubu_cmap.anchorToBottom( &orrd_cmap );
    pubu_cmap.show();

    // multi-hue PuBuGn
    kvs::ColorMapBar pubugn_cmap( &screen );
    pubugn_cmap.setCaption( "BrewerMultiHuePuBuGn" );
    pubugn_cmap.setColorMap( kvs::ColorMap::BrewerMultiHuePuBuGn( resolution ) );
    pubugn_cmap.setColorBarWidth( width );
    pubugn_cmap.setHeight( height );
    pubugn_cmap.setMargin( margin );
    pubugn_cmap.hideRange();
    pubugn_cmap.anchorToBottom( &pubu_cmap );
    pubugn_cmap.show();

    //--------------------------------------------------------------------------

    // multi-hue PuRd
    kvs::ColorMapBar purd_cmap( &screen );
    purd_cmap.setCaption( "BrewerMultiHuePuRd" );
    purd_cmap.setColorMap( kvs::ColorMap::BrewerMultiHuePuRd( resolution ) );
    purd_cmap.setColorBarWidth( width );
    purd_cmap.setHeight( height );
    purd_cmap.setMargin( margin );
    purd_cmap.hideRange();
    purd_cmap.anchorToRight( &bugn_cmap );
    purd_cmap.show();

    // multi-hue RdPu
    kvs::ColorMapBar rdpu_cmap( &screen );
    rdpu_cmap.setCaption( "BrewerMultiHueRdPu" );
    rdpu_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueRdPu( resolution ) );
    rdpu_cmap.setColorBarWidth( width );
    rdpu_cmap.setHeight( height );
    rdpu_cmap.setMargin( margin );
    rdpu_cmap.hideRange();
    rdpu_cmap.anchorToBottom( &purd_cmap );
    rdpu_cmap.show();

    // multi-hue YlGn
    kvs::ColorMapBar ylgn_cmap( &screen );
    ylgn_cmap.setCaption( "BrewerMultiHueYlGn" );
    ylgn_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueYlGn( resolution ) );
    ylgn_cmap.setColorBarWidth( width );
    ylgn_cmap.setHeight( height );
    ylgn_cmap.setMargin( margin );
    ylgn_cmap.hideRange();
    ylgn_cmap.anchorToBottom( &rdpu_cmap );
    ylgn_cmap.show();

    // multi-hue YlGnBu
    kvs::ColorMapBar ylgnbu_cmap( &screen );
    ylgnbu_cmap.setCaption( "BrewerMultiHueYlGnBu" );
    ylgnbu_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueYlGnBu( resolution ) );
    ylgnbu_cmap.setColorBarWidth( width );
    ylgnbu_cmap.setHeight( height );
    ylgnbu_cmap.setMargin( margin );
    ylgnbu_cmap.hideRange();
    ylgnbu_cmap.anchorToBottom( &ylgn_cmap );
    ylgnbu_cmap.show();

    // multi-hue YlOrBr
    kvs::ColorMapBar ylorbr_cmap( &screen );
    ylorbr_cmap.setCaption( "BrewerMultiHueYlOrBr" );
    ylorbr_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueYlOrBr( resolution ) );
    ylorbr_cmap.setColorBarWidth( width );
    ylorbr_cmap.setHeight( height );
    ylorbr_cmap.setMargin( margin );
    ylorbr_cmap.hideRange();
    ylorbr_cmap.anchorToBottom( &ylgnbu_cmap );
    ylorbr_cmap.show();

    // multi-hue YlOrRd
    kvs::ColorMapBar ylorrd_cmap( &screen );
    ylorrd_cmap.setCaption( "BrewerMultiHueYlOrRd" );
    ylorrd_cmap.setColorMap( kvs::ColorMap::BrewerMultiHueYlOrRd( resolution ) );
    ylorrd_cmap.setColorBarWidth( width );
    ylorrd_cmap.setHeight( height );
    ylorrd_cmap.setMargin( margin );
    ylorrd_cmap.hideRange();
    ylorrd_cmap.anchorToBottom( &ylorbr_cmap );
    ylorrd_cmap.show();

    //--------------------------------------------------------------------------

    // Viridis
    kvs::ColorMapBar viridis_cmap( &screen );
    viridis_cmap.setCaption( "Viridis" );
    viridis_cmap.setColorMap( kvs::ColorMap::Viridis( resolution ) );
    viridis_cmap.setColorBarWidth( width );
    viridis_cmap.setHeight( height );
    viridis_cmap.setMargin( margin );
    viridis_cmap.hideRange();
    viridis_cmap.anchorToRight( &purd_cmap );
    viridis_cmap.show();

    // Plasma
    kvs::ColorMapBar plasma_cmap( &screen );
    plasma_cmap.setCaption( "Plasma" );
    plasma_cmap.setColorMap( kvs::ColorMap::Plasma( resolution ) );
    plasma_cmap.setColorBarWidth( width );
    plasma_cmap.setHeight( height );
    plasma_cmap.setMargin( margin );
    plasma_cmap.hideRange();
    plasma_cmap.anchorToBottom( &viridis_cmap );
    plasma_cmap.show();

    // Inferno
    kvs::ColorMapBar inferno_cmap( &screen );
    inferno_cmap.setCaption( "Inferno" );
    inferno_cmap.setColorMap( kvs::ColorMap::Inferno( resolution ) );
    inferno_cmap.setColorBarWidth( width );
    inferno_cmap.setHeight( height );
    inferno_cmap.setMargin( margin );
    inferno_cmap.hideRange();
    inferno_cmap.anchorToBottom( &plasma_cmap );
    inferno_cmap.show();

    // Magma
    kvs::ColorMapBar magma_cmap( &screen );
    magma_cmap.setCaption( "Magma" );
    magma_cmap.setColorMap( kvs::ColorMap::Magma( resolution ) );
    magma_cmap.setColorBarWidth( width );
    magma_cmap.setHeight( height );
    magma_cmap.setMargin( margin );
    magma_cmap.hideRange();
    magma_cmap.anchorToBottom( &inferno_cmap );
    magma_cmap.show();

    // Cividis
    kvs::ColorMapBar cividis_cmap( &screen );
    cividis_cmap.setCaption( "Cividis" );
    cividis_cmap.setColorMap( kvs::ColorMap::Cividis( resolution ) );
    cividis_cmap.setColorBarWidth( width );
    cividis_cmap.setHeight( height );
    cividis_cmap.setMargin( margin );
    cividis_cmap.hideRange();
    cividis_cmap.anchorToBottom( &magma_cmap );
    cividis_cmap.show();

    // Original
    const auto c = kvs::RGBColor( 207,  85,  62 );
    kvs::ColorMapBar original_cmap( &screen );
    original_cmap.setCaption( "Sequential" );
    original_cmap.setColorMap( kvs::ColorMap::Sequential( c, resolution ) );
    original_cmap.setColorBarWidth( width );
    original_cmap.setHeight( height );
    original_cmap.setMargin( margin );
    original_cmap.hideRange();
    original_cmap.anchorToBottom( &cividis_cmap );
    original_cmap.show();

    return app.run();
}
