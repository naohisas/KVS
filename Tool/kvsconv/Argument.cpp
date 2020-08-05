/*****************************************************************************/
/**
 *  @file   Argument.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Argument.h"
#include "CommandName.h"
#include "FldConv.h"
#include "UcdConv.h"
#include "ImgConv.h"
#include "TetConv.h"


namespace kvsconv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvs::CommandLine( argc, argv, kvsconv::CommandName )
{
    addHelpOption();

    // Help option for each converter.
    addOption("help", "Output help message for each converter. (ex. -help fld_conv)", 1 );

    // Converter commands.
    addOption( kvsconv::FldConv::CommandName, kvsconv::FldConv::Description, 0 );
    addOption( kvsconv::UcdConv::CommandName, kvsconv::UcdConv::Description, 0 );
    addOption( kvsconv::TetConv::CommandName, kvsconv::TetConv::Description, 0 );
    addOption( kvsconv::ImgConv::CommandName, kvsconv::ImgConv::Description, 0 );

    // Input value.
    addValue( "input data", false );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new common argument class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Common::Common( int argc, char** argv ):
    kvs::CommandLine( argc, argv, kvsconv::CommandName )
{
    this->set_options();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument::Common class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @param  converter [in] converter name
 */
/*===========================================================================*/
Argument::Common::Common( int argc, char** argv, const std::string& converter ):
    kvs::CommandLine( argc, argv, kvsconv::CommandName + " -" + converter )
{
    this->set_options();
}

/*===========================================================================*/
/**
 *  @brief  Set options for the common argument.
 */
/*===========================================================================*/
void Argument::Common::set_options()
{
    addHelpOption( "help" );
    addOption( "output", "Output filename. (default: <basename of input data file>.<default extension>)", 1, false );
    addValue( "input data file" );
}

} // end of namespace kvsconv
