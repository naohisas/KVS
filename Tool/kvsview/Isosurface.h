/*****************************************************************************/
/**
 *  @file   Isosurface.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
#include <kvs/PolygonObject>
#include <kvs/VolumeObjectBase>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace Isosurface
{

const std::string CommandName("Isosurface");
const std::string Description("Extract surfaces using Isosurface. (optional)");

/*===========================================================================*/
/**
 *  Argument class for Isosurface.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const kvs::Real64 isolevel( const kvs::VolumeObjectBase* volume, const kvs::TransferFunction& transfer_function );
    const kvs::PolygonObject::NormalType normalType();
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for Isosurface.
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

} // end of namespace Isosurface

} // end of namespace kvsview
