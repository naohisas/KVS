/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::KMeansClustering class.
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ValueTable>
#include <kvs/ValueArray>
#include <kvs/MersenneTwister>
#include <kvs/KMeansClustering>
#include <kvs/TableObject>
#include <kvs/ScatterPlotRenderer>
#include <kvs/Axis2D>


/*===========================================================================*/
/**
 *  @brief  Creates value table.
 *  @param  nrows [in] number of rows
 */
/*===========================================================================*/
kvs::ValueTable<kvs::Real32> CreateValueTable( const size_t nrows )
{
    kvs::Real32 xmin = 2.0f;
    kvs::Real32 xmax = 6.0f;
    kvs::Real32 ymin = 2.0f;
    kvs::Real32 ymax = 6.0f;

    kvs::ValueArray<kvs::Real32> x( nrows );
    kvs::ValueArray<kvs::Real32> y( nrows );

    kvs::MersenneTwister R;
    for ( size_t i = 0; i < nrows / 4; i++ )
    {
        x[i] = -( R() * ( xmax - xmin ) + xmin );
        y[i] = +( R() * ( ymax - ymin ) + ymin );
    }

    for ( size_t i = nrows / 4; i < nrows / 4 * 2; i++ )
    {
        x[i] = -( R() * ( xmax - xmin ) + xmin );
        y[i] = -( R() * ( ymax - ymin ) + ymin );
    }

    for ( size_t i = nrows / 4 * 2; i < nrows / 4 * 3; i++ )
    {
        x[i] = +( R() * ( xmax - xmin ) + xmin );
        y[i] = -( R() * ( ymax - ymin ) + ymin );
    }

    for ( size_t i = nrows / 4 * 3; i < nrows; i++ )
    {
        x[i] = +( R() * ( xmax - xmin ) + xmin );
        y[i] = +( R() * ( ymax - ymin ) + ymin );
    }

    kvs::ValueTable<kvs::Real32> table;
    table.pushBackColumn( x );
    table.pushBackColumn( y );

    return table;
}

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.setTitle( "k-means clsutering" );
    screen.setSize( 600, 600 );
    screen.show();

    const size_t nrows = 1000;
    auto data = CreateValueTable( nrows );
    auto* table = new kvs::TableObject();
    table->setTable( data );
    table->setMinValue( 0, -7 );
    table->setMaxValue( 0,  7 );
    table->setMinValue( 1, -7 );
    table->setMaxValue( 1,  7 );

    kvs::ColorMap cmap( 4 );
    cmap.addPoint( 0, kvs::UIColor::Red() );
    cmap.addPoint( 1, kvs::UIColor::Green() );
    cmap.addPoint( 2, kvs::UIColor::Yellow() );
    cmap.addPoint( 3, kvs::UIColor::Indigo() );
    cmap.create();

    // Original
    {
        auto* renderer = new kvs::ScatterPlotRenderer();
        renderer->setPointSize( 4.0 );
        renderer->setPointColor( kvs::UIColor::Gray() );

        auto* axis = new kvs::Axis2D();
        axis->setTitle( "Original" );
        axis->xAxis().setNumberOfTicks( 3 );
        axis->yAxis().setNumberOfTicks( 3 );

        const int right_margin = screen.width() / 2 + axis->rightMargin();
        const int bottom_margin = screen.height() / 2 + axis->bottomMargin();
        renderer->setRightMargin( right_margin );
        renderer->setBottomMargin( bottom_margin );
        axis->setRightMargin( right_margin );
        axis->setBottomMargin( bottom_margin );

        screen.registerObject( table, axis );
        screen.registerObject( table, renderer );
    }

    // Random Seeding.
    {
        const int nclusters = 4;
        auto* kmeans = new kvs::KMeansClustering();
        kmeans->setClusteringMethod( kvs::KMeansClustering::FastKMeans );
        kmeans->setSeedingMethod( kvs::KMeansClustering::RandomSeeding );
        kmeans->setNumberOfClusters( nclusters );

        auto* object = kmeans->exec( table );
        object->setMinValue( 0, -7 );
        object->setMaxValue( 0,  7 );
        object->setMinValue( 1, -7 );
        object->setMaxValue( 1,  7 );

        auto* renderer = new kvs::ScatterPlotRenderer();
        renderer->setPointSize( 4.0 );
        renderer->setColorMap( cmap );

        auto* axis = new kvs::Axis2D();
        axis->setTitle( "Random Seeding" );
        axis->xAxis().setNumberOfTicks( 3 );
        axis->yAxis().setNumberOfTicks( 3 );

        const int left_margin = screen.width() / 2 + axis->leftMargin();
        const int bottom_margin = screen.height() / 2 + axis->bottomMargin();
        renderer->setLeftMargin( left_margin );
        renderer->setBottomMargin( bottom_margin );
        axis->setLeftMargin( left_margin );
        axis->setBottomMargin( bottom_margin );

        screen.registerObject( object, axis );
        screen.registerObject( object, renderer );
    }

    // Smart Seeding.
    {
        const int nclusters = 4;
        auto* kmeans = new kvs::KMeansClustering();
        kmeans->setClusteringMethod( kvs::KMeansClustering::FastKMeans );
        kmeans->setSeedingMethod( kvs::KMeansClustering::SmartSeeding );
        kmeans->setNumberOfClusters( nclusters );

        auto* object = kmeans->exec( table );
        object->setMinValue( 0, -7 );
        object->setMaxValue( 0,  7 );
        object->setMinValue( 1, -7 );
        object->setMaxValue( 1,  7 );

        auto* renderer = new kvs::ScatterPlotRenderer();
        renderer->setPointSize( 4.0 );
        renderer->setColorMap( cmap );

        auto* axis = new kvs::Axis2D();
        axis->setTitle( "Smart Seeding" );
        axis->xAxis().setNumberOfTicks( 3 );
        axis->yAxis().setNumberOfTicks( 3 );

        const int right_margin = screen.width() / 2 + axis->rightMargin();
        const int top_margin = screen.height() / 2 + axis->topMargin();
        renderer->setRightMargin( right_margin );
        renderer->setTopMargin( top_margin );
        axis->setRightMargin( right_margin );
        axis->setTopMargin( top_margin );

        screen.registerObject( object, axis );
        screen.registerObject( object, renderer );
    }

    // Auto-estimation
    {
        const int max_nclusters = 10;
        auto* kmeans = new kvs::KMeansClustering();
        kmeans->setClusteringMethod( kvs::KMeansClustering::AdaptiveKMeans );
        kmeans->setNumberOfClusters( max_nclusters );

        auto* object = kmeans->exec( table );
        object->setMinValue( 0, -7 );
        object->setMaxValue( 0,  7 );
        object->setMinValue( 1, -7 );
        object->setMaxValue( 1,  7 );

        auto* renderer = new kvs::ScatterPlotRenderer();
        renderer->setPointSize( 4.0 );
        renderer->setColorMap( cmap );

        auto* axis = new kvs::Axis2D();
        axis->setTitle( "Auto-estimation" );
        axis->xAxis().setNumberOfTicks( 3 );
        axis->yAxis().setNumberOfTicks( 3 );

        const int left_margin = screen.width() / 2 + axis->leftMargin();
        const int top_margin = screen.height() / 2 + axis->topMargin();
        renderer->setLeftMargin( left_margin );
        renderer->setTopMargin( top_margin );
        axis->setLeftMargin( left_margin );
        axis->setTopMargin( top_margin );

        screen.registerObject( object, axis );
        screen.registerObject( object, renderer );
    }

    return app.run();
}
