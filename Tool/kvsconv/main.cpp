/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/MemoryDebugger>
#include <kvs/Message>
#include "main.h"
#include "Argument.h"
#include "FldConv.h"
#include "UcdConv.h"
#include "TetConv.h"
#include "ImgConv.h"

KVS_MEMORY_DEBUGGER;

#define KVSCONV_HELP( command )                                         \
    if ( c == command::CommandName ) return command::Argument( m_argc, m_argv ).parse()

#define KVSCONV_EXEC( command )                                         \
    if ( arg.hasOption( command::CommandName ) ) return command::Main( m_argc, m_argv ).exec()


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
        const auto c = arg.optionValue<std::string>("help");
        KVSCONV_HELP( FldConv );
        KVSCONV_HELP( UcdConv );
        KVSCONV_HELP( TetConv );
        KVSCONV_HELP( ImgConv );
        kvsMessageError() << "Unknown converter '" << c << "'." << std::endl;;
        return false;
    }

    // Convert.
    KVSCONV_EXEC( FldConv );
    KVSCONV_EXEC( UcdConv );
    KVSCONV_EXEC( TetConv );
    KVSCONV_EXEC( ImgConv );
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
