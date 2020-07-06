/*****************************************************************************/
/**
 *  @file   ExtractVertices.h
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

namespace ExtractVertices
{

const std::string CommandName("ExtractVertices");
const std::string Description("Extract vertices of the volume data. (optional)");

/*===========================================================================*/
/**
 *  Argument class for ExtractVertices.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const kvs::Real32 size() { return valueAs<kvs::Real32>( "s", 0.0f ); }
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for ExtractVertices.
 */
/*===========================================================================*/
class Main : public kvs::Program
{
private:
    std::string m_input_name; ///< input filename
    std::string m_output_name; ///< output filename

public:
    int exec( int argc, char** argv );
};

} // end of namespace ExtractVertices

} // end of namespace kvsview
