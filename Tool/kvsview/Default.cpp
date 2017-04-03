/*****************************************************************************/
/**
 *  @file   Default.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Default.cpp 1497 2013-04-04 07:17:54Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Default.h"
#include <kvs/DebugNew>
#include <kvs/XMLDocument>
#include <kvs/XMLNode>
#include <kvs/KVSMLTransferFunction>
#include <kvs/VisualizationPipeline>
#include <kvs/ImageObject>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
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
    if ( kvs::KVSMLTransferFunction::CheckExtension( filename ) )
    {
        // Find a TransferFunction tag without error messages.
        kvs::XMLDocument document;
        if ( !document.read( filename ) ) return false;

        // <KVSML>
        const std::string kvsml_tag("KVSML");
        const kvs::XMLNode::SuperClass* kvsml_node = kvs::XMLDocument::FindNode( &document, kvsml_tag );
        if ( !kvsml_node ) return false;

        // <TransferFunction>
        const std::string tfunc_tag("TransferFunction");
        const kvs::XMLNode::SuperClass* tfunc_node = kvs::XMLNode::FindChildNode( kvsml_node, tfunc_tag );
        if ( !tfunc_node ) return false;

        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a default viewer.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsview::Argument::Common( argc, argv )
{
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int Main::exec( int argc, char** argv )
{
    // GLUT application.
    kvs::glut::Application app( argc, argv );

    // Parse specified arguments.
    kvsview::Default::Argument arg( argc, argv );
    if ( !arg.parse() ) { return false; }

    /* Transfer function data is checked here, since default visualization
     * method for the transfer function data hasn't yet been implemented in
     * the visualization pipeline class.
     */
    if ( kvsview::Default::CheckTransferFunctionFormat( arg.value<std::string>() ) )
    {
        return kvsview::TransferFunction::Main().start( argc, argv );
    }

    // Create a global and screen class.
    kvs::glut::Screen screen( &app );
    screen.setSize( 512, 512 );
    screen.setTitle("kvsview - Default");
    screen.show();

    // Visualization pipeline.
    m_input_name = arg.value<std::string>();
    kvs::VisualizationPipeline pipe( m_input_name );
    pipe.import();

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "IMPORTED OBJECT" << std::endl, kvs::Indent(4) );
    }

    if ( pipe.object()->objectType() == kvs::ObjectBase::Volume )
    {
        const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( pipe.object() );
        if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
        {
            kvs::PipelineModule renderer( new kvs::glsl::RayCastingRenderer );
            pipe.connect( renderer );
        }
    }

    if ( !pipe.exec() )
    {
        kvsMessageError("Cannot execute the visulization pipeline.");
        return( false );
    }
    screen.registerObject( &pipe );

    // Verbose information.
    if ( arg.verboseMode() )
    {
        pipe.object()->print( std::cout << std::endl << "RENDERERED OBJECT" << std::endl, kvs::Indent(4) );
        pipe.print(  std::cout << std::endl << "VISUALIZATION PIPELINE" << std::endl, kvs::Indent(4) );
    }

    // Apply the specified parameters to the global and the visualization pipeline.
    arg.applyTo( screen, pipe );
    arg.applyTo( screen );

    // In case of the image object, the screen size is equal to the image size.
    if ( pipe.object()->objectType() == kvs::ObjectBase::Image )
    {
        const kvs::ImageObject* image = reinterpret_cast<const kvs::ImageObject*>( pipe.object() );
        const size_t width = image->width();
        const size_t height = image->height();
        screen.setSize( width, height );
    }

    return arg.clear(), app.run();
}

} // end of namespace Default

} // end of namespace kvsview
