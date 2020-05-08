/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::KMeansClustering class.
 */
/*****************************************************************************/
#include <kvs/ValueTable>
#include <kvs/ValueArray>
#include <kvs/MersenneTwister>
#include <kvs/KMeansClustering>
#include <kvs/TableObject>
#include <kvs/ScatterPlotRenderer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Axis2D>


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
 *  @brief  Executes clustering process with random seeding.
 *  @param  screen [in] screen
 *  @param  table [in] pointer to the table object
 *  @param  nclusters [in] number of clusters
 */
/*===========================================================================*/
void RandomSeeding( kvs::glut::Screen& screen, kvs::TableObject* table, size_t nclusters )
{
    kvs::KMeansClustering* kmeans = new kvs::KMeansClustering();
    kmeans->setClusteringMethod( kvs::KMeansClustering::FastKMeans );
    kmeans->setSeedingMethod( kvs::KMeansClustering::RandomSeeding );
    kmeans->setNumberOfClusters( nclusters );

    kvs::TableObject* object = kmeans->exec( table );
    kvs::ScatterPlotRenderer* renderer = new kvs::ScatterPlotRenderer();
    renderer->setPointSize( 15.0 );

    screen.setTitle( "k-means (random seeding)" );
    screen.setBackgroundColor( kvs::RGBColor( 255, 255, 255 ) );
    screen.registerObject( object, new kvs::glut::Axis2D() );
    screen.registerObject( object, renderer );
    screen.show();
}

/*===========================================================================*/
/**
 *  @brief  Executes clustering process with smart seeding.
 *  @param  screen [in] screen
 *  @param  table [in] pointer to the table object
 *  @param  nclusters [in] number of clusters
 */
/*===========================================================================*/
void SmartSeeding( kvs::glut::Screen& screen, kvs::TableObject* table, size_t nclusters )
{
    kvs::KMeansClustering* kmeans = new kvs::KMeansClustering();
    kmeans->setClusteringMethod( kvs::KMeansClustering::FastKMeans );
    kmeans->setSeedingMethod( kvs::KMeansClustering::SmartSeeding );
    kmeans->setNumberOfClusters( nclusters );

    kvs::TableObject* object = kmeans->exec( table );
    kvs::ScatterPlotRenderer* renderer = new kvs::ScatterPlotRenderer();
    renderer->setPointSize( 15.0 );

    screen.setTitle( "k-means (smart seeding)" );
    screen.setBackgroundColor( kvs::RGBColor( 255, 255, 255 ) );
    screen.registerObject( object, new kvs::glut::Axis2D() );
    screen.registerObject( object, renderer );
    screen.show();
}

/*===========================================================================*/
/**
 *  @brief  Executes clustering process with k auto-estimation.
 *  @param  screen [in] screen
 *  @param  table [in] pointer to the table object
 *  @param  nclusters [in] number of clusters
 */
/*===========================================================================*/
void KAutoEstimation( kvs::glut::Screen& screen, kvs::TableObject* table, size_t max_nclusters )
{
    kvs::KMeansClustering* kmeans = new kvs::KMeansClustering();
    kmeans->setClusteringMethod( kvs::KMeansClustering::AdaptiveKMeans );
    kmeans->setNumberOfClusters( max_nclusters );

    kvs::TableObject* object = kmeans->exec( table );
    kvs::ScatterPlotRenderer* renderer = new kvs::ScatterPlotRenderer();
    renderer->setPointSize( 15.0 );

    screen.setTitle( "k-means (k auto-estimation)" );
    screen.setBackgroundColor( kvs::RGBColor( 255, 255, 255 ) );
    screen.registerObject( object, new kvs::glut::Axis2D() );
    screen.registerObject( object, renderer );
    screen.show();
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
    kvs::glut::Screen screen1( &app );
    kvs::glut::Screen screen2( &app );
    kvs::glut::Screen screen3( &app );

    screen1.setPosition(   0, 0 );
    screen2.setPosition(  50, 0 );
    screen3.setPosition( 100, 0 );

    size_t nrows = 1000;
    kvs::ValueTable<kvs::Real32> data = CreateValueTable( nrows );

    kvs::TableObject* table = new kvs::TableObject();
    table->setTable( data );

    RandomSeeding( screen1, table, 4 );
    SmartSeeding( screen2, table, 4 );
    KAutoEstimation( screen3, table, 10 );

    delete table;

    return app.run();
}
