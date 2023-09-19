/*****************************************************************************/
/**
 *  @file   Default.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Default.h"
#include <kvs/DebugNew>
#include <kvs/XMLDocument>
#include <kvs/XMLNode>
#include <kvs/KVSMLTransferFunction>
#include <kvs/VisualizationPipeline>
#include <kvs/ImageObject>
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/RayCastingRenderer>
#include "TransferFunction.h"


namespace kvsview
{

namespace Default
{

/*===========================================================================*/
/**
 *  @brief  Checks whether the given file is KVSML transfer function format or not.
 *  @param  filename [in] filename
 *  @return true, if the given file is KVSML transfer function format
 */
/*===========================================================================*/
const bool CheckTransferFunctionFormat( const std::string& filename )
{
    if ( !kvs::KVSMLTransferFunction::CheckExtension( filename ) ) { return false; }

    // Find a TransferFunction tag without error messages.
    kvs::XMLDocument document;
    if ( !document.read( filename ) ) { return false; }

    // <KVSML>
    const std::string kvsml_tag("KVSML");
    const auto* kvsml_node = kvs::XMLDocument::FindNode( &document, kvsml_tag );
    if ( !kvsml_node ) { return false; }

    // <TransferFunction>
    const std::string tfunc_tag("TransferFunction");
    const auto* tfunc_node = kvs::XMLNode::FindChildNode( kvsml_node, tfunc_tag );
    if ( !tfunc_node ) { return false; }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int Main::exec()
{
    // Application.
    kvs::Application app( m_argc, m_argv );

    // Parse specified arguments.
    kvsview::Default::Argument arg( m_argc, m_argv );
    if ( !arg.parse() ) { return false; }

    /* Transfer function data is checked here, since default visualization
     * method for the transfer function data hasn't yet been implemented in
     * the visualization pipeline class.
     */
    if ( kvsview::Default::CheckTransferFunctionFormat( arg.value<std::string>() ) )
    {
        return kvsview::TransferFunction::Main( m_argc, m_argv ).run();
    }

    // Create a global and screen class.
    kvs::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle( "kvsview - Default" );

    // Visualization pipeline.
    m_input_name = arg.value<std::string>();
    kvs::VisualizationPipeline pipe( m_input_name );
    pipe.import();

    // Verbose information.
    const kvs::Indent indent( 4 );
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, indent );
    }

    if ( pipe.object()->objectType() == kvs::ObjectBase::Volume )
    {
        const auto* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
        if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
        {
            kvs::PipelineModule renderer( new kvs::glsl::RayCastingRenderer );
            pipe.connect( renderer );
        }
    }

    // Pipeline execution.
    if ( !pipe.exec() )
    {
        kvsMessageError() << "Cannot execute the visulization pipeline." << std::endl;
        return ( false );
    }
    screen.registerObject( &pipe );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, indent );
        pipe.print(  std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, indent );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // In case of the image object, the screen size is equal to the image size.
    if ( pipe.object()->objectType() == kvs::ObjectBase::Image )
    {
        const auto* image = kvs::ImageObject::DownCast( pipe.object() );
        const auto width = image->width();
        const auto height = image->height();
        screen.setSize( width, height );
    }

    screen.show();
    return arg.clear(), app.run();
}

} // end of namespace Default

} // end of namespace kvsview
