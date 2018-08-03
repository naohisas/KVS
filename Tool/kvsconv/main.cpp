/*****************************************************************************/
/**
 *  @file   main.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <kvs/MemoryDebugger>
#include "main.h"
#include "Argument.h"
#include "fld2kvsml.h"
#include "ucd2kvsml.h"
#include "img2img.h"
#include "tet2tet.h"
#include <kvs/Message>

KVS_MEMORY_DEBUGGER;


namespace kvsconv
{

/*===========================================================================*/
/**
 *  @brief  Execute main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Read the argument.
    Argument arg( m_argc, m_argv );
    if ( !arg.read() ) return false;

    // Output help message for each converter.
    if ( arg.hasOption("help") )
    {
        const std::string c = arg.optionValue<std::string>("help");
        if ( c == "fld2kvsml" ) return fld2kvsml::Argument( m_argc, m_argv ).parse();
        if ( c == "ucd2kvsml" ) return ucd2kvsml::Argument( m_argc, m_argv ).parse();
        if ( c == "img2img" ) return img2img::Argument( m_argc, m_argv ).parse();
        if ( c == "tet2tet" ) return tet2tet::Argument( m_argc, m_argv ).parse();

        kvsMessageError( "Unknown converter %s.", c.c_str() );
        return false;
    }

    // Convert.
    if ( arg.hasOption("fld2kvsml") ) return fld2kvsml::Main( m_argc, m_argv ).exec();
    if ( arg.hasOption("ucd2kvsml") ) return ucd2kvsml::Main( m_argc, m_argv ).exec();
    if ( arg.hasOption("img2img") ) return img2img::Main( m_argc, m_argv ).exec();
    if ( arg.hasOption("tet2tet") ) return tet2tet::Main( m_argc, m_argv ).exec();

    return false;
}

} // end of namespace kvsconv


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    KVS_MEMORY_DEBUGGER__SET_ARGUMENT( argc, argv );

    kvsconv::Main m( argc, argv );
    return m.exec();
}
