/*****************************************************************************/
/**
 *  @file   ExternalFaces.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/TransferFunction>
#include <kvs/Program>
#include "Argument.h"


namespace kvsview
{

namespace ExternalFaces
{

const std::string CommandName("ExternalFaces");
const std::string Description("Extract external faces of the volume data. (optional)");

/*===========================================================================*/
/**
 *  Argument class for ExternalFaces.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const kvs::UInt8 opacity() { return valueAs<kvs::UInt8>( "a", kvs::UInt8(255) ); }
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for ExternalFaces.
 */
/*===========================================================================*/
class Main : public kvs::Program
{
private:
    std::string m_input_name; ///< input filename
    std::string m_output_name; ///< output filename
    int m_argc;
    char** m_argv;

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    int exec();
};

} // end of namespace ExternalFaces

} // end of namespace kvsview
