/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::StochasticMultivariateTetrahedraRenderer class.
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/RGBFormulae>
#include <kvs/TransferFunction>
#include <kvs/StochasticMultivariateTetrahedraRenderer>
#include <iostream>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    kvs::UnstructuredVolumeObject* object = new kvs::UnstructuredVolumeImporter( argv[1] );
    object->print( std::cout );

    kvs::StochasticMultivariateTetrahedraRenderer* renderer = new kvs::StochasticMultivariateTetrahedraRenderer();
    renderer->setRepetitionLevel( 50 );
    renderer->enableLODControl();

    kvs::TransferFunction tfunc0( kvs::RGBFormulae::PM3D( 256 ) );
    kvs::TransferFunction tfunc1( kvs::RGBFormulae::Rainbow( 256 ) );
    renderer->setTransferFunction( tfunc0, 0 );
    renderer->setTransferFunction( tfunc1, 1 );

    kvs::glut::Screen screen( &app );
    screen.setTitle("Example program for kvs::StochasticMultivariateTetrahedraRenderer");
    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
