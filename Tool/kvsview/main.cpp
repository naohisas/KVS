/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1497 2013-04-04 07:17:54Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/MemoryDebugger>
#include <kvs/Message>
#include "main.h"
#include "Argument.h"
#include "Default.h"
#include "PointRenderer.h"
#include "LineRenderer.h"
#include "PolygonRenderer.h"
#include "Isosurface.h"
#include "SlicePlane.h"
#include "OrthoSlice.h"
#include "TransferFunction.h"
#include "ExtractEdges.h"
#include "ExtractVertices.h"
#include "ExternalFaces.h"
#include "RayCastingRenderer.h"
#include "ParticleBasedRenderer.h"
#include "Histogram.h"


KVS_MEMORY_DEBUGGER;

#define KVSVIEW_HELP( method )                                          \
    if ( help == #method ) { return method ::Argument( argc, argv ).parse(); }

#define KVSVIEW_EXEC( method )                                          \
    if ( arg.hasOption( #method ) ) { return ( arg.clear(), method ::Main().start( argc, argv ) ); }


namespace kvsview
{

/*===========================================================================*/
/**
 *  @brief  Execute main process.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int32_t Main::exec( int argc, char** argv )
{
    Argument arg( argc, argv );
    if ( !arg.read() ) { return 1; }

    // Output help messsage for the specified visualization method.
    if ( arg.hasOption("help") )
    {
        const std::string help = arg.optionValue<std::string>("help");
        KVSVIEW_HELP( Default );
        KVSVIEW_HELP( PointRenderer );
        KVSVIEW_HELP( LineRenderer );
        KVSVIEW_HELP( PolygonRenderer );
        KVSVIEW_HELP( Isosurface );
        KVSVIEW_HELP( SlicePlane );
        KVSVIEW_HELP( OrthoSlice );
        KVSVIEW_HELP( TransferFunction );
        KVSVIEW_HELP( ExtractEdges );
        KVSVIEW_HELP( ExtractVertices );
        KVSVIEW_HELP( ExternalFaces );
        KVSVIEW_HELP( RayCastingRenderer );
        KVSVIEW_HELP( ParticleBasedRenderer );
        KVSVIEW_HELP( Histogram );
        kvsMessageError( "Unknown visualization method '%s'.", help.c_str() );
        return 1;
    }

    // Execute the specified visualization method.
    else
    {
        KVSVIEW_EXEC( Default );
        KVSVIEW_EXEC( PointRenderer );
        KVSVIEW_EXEC( LineRenderer );
        KVSVIEW_EXEC( PolygonRenderer );
        KVSVIEW_EXEC( Isosurface );
        KVSVIEW_EXEC( SlicePlane );
        KVSVIEW_EXEC( OrthoSlice );
        KVSVIEW_EXEC( TransferFunction );
        KVSVIEW_EXEC( ExtractEdges );
        KVSVIEW_EXEC( ExtractVertices );
        KVSVIEW_EXEC( ExternalFaces );
        KVSVIEW_EXEC( RayCastingRenderer );
        KVSVIEW_EXEC( ParticleBasedRenderer );
        KVSVIEW_EXEC( Histogram );
        return ( arg.clear(), Default::Main().start( argc, argv ) );
    }
}

} // end of namespace kvsview


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    KVS_MEMORY_DEBUGGER__SET_ARGUMENT( argc, argv );
    kvsview::Main program;
    return program.exec( argc, argv );
}
