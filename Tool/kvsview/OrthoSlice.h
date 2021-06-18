/*****************************************************************************/
/**
 *  @file   OrthoSlice.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
#include <kvs/PolygonObject>
#include <kvs/TransferFunction>
#include <kvs/OrthoSlice>
#include "Argument.h"


namespace kvsview
{

namespace OrthoSlice
{

const std::string CommandName("OrthoSlice");
const std::string Description("Extract a orthogonal slice plane. (optional)");

/*===========================================================================*/
/**
 *  Argument class for OrthoSlice.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const float position() { return valueAs<float>( "p", 0.0f ); }
    const kvs::OrthoSlice::AlignedAxis axis();
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for OrthoSlice.
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

} // end of namespace OrthoSlice

} // end of namespace kvsview
