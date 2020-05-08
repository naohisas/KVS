/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::VisualizationPipeline class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/VisualizationPipeline>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return 0, if the process is done successfully.
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Visualization pipeline.
    const std::string filename( argc > 1 ? argv[1] : "" );
    kvs::VisualizationPipeline pipeline( filename );

    /* Execute the visualization pipeline by caling the 'exec' method.
     *
     * You can confirm whether the pipeline is executed successfully or not
     * by checking the return value of the 'exec' method.
     *
     * In case that any pipeline modules is connected to the pipeline, the
     * suitable file format class, importer class and renderer class are
     * automatically estimated and connected. If you want to use some KVS module,
     * for example kvs::MarchingCubes to extract surfaces from a volume object,
     * the pipeline can be connected as follows:
     *
     *    // Input parameters for Marching cubes.
     *    double isolevel = 100;
     *    kvs::PolygonObject::NormalType normal = kvs::PolygonObject::PolygonNormal;
     *    kvs::TransferFunction transfer_function( 256 );
     *    // Create a marching cubes module.
     *    kvs::PipelineModule mapper( new kvs::MarchingCubes );
     *    mapper.get<kvs::MarchingCubes>()->setTransferFunction( transfer_function );
     *    mapper.get<kvs::MarchingCubes>()->setNormalType( normal );
     *    mapper.get<kvs::MarchingCubes>()->setIsolevel( isolevel );
     *    // Connect to the pipeline.
     *    pipeline.connect( mapper );
     */
    if ( !pipeline.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return( false );
    }

    // Output the visualization pipeline as a string.
    pipeline.print();

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.registerObject( &pipeline );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::VisualizationPipeline" );
    screen.show();

    return( app.run() );
}
