/*****************************************************************************/
/**
 *  @file   RayCastingRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
#include <kvs/TransferFunction>
#include <kvs/VolumeObjectBase>
#include "Argument.h"


namespace kvsview
{

namespace RayCastingRenderer
{

const std::string CommandName( "RayCastingRenderer" );
const std::string Description( "Rendering a volume object. (optional)" );

/*===========================================================================*/
/**
 *  Argument class for RayCastingRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

    int shader() const { return valueAs<int>( "shader", 0 ); }
    bool noshading() const { return hasOption( "noshading" ); }
    bool nolod() const { return hasOption( "nolod" ); }
    bool nogpu() const { return hasOption( "nogpu" ); }
    bool jittering() const { return hasOption( "jittering" ); }
    float ambient() const { return valueAs<float>( "ka", shader() == 0 ? 0.4f : 0.3f ); }
    float diffuse() const { return valueAs<float>( "kd", shader() == 0 ? 0.6f : 0.5f ); }
    float specular() const { return valueAs<float>( "ks", 0.8f ); }
    float shininess() const { return valueAs<float>( "n", 100.0f ); }
    float step() const { return valueAs<float>( "step", 0.5f ); }

    kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume ) const;
};

/*===========================================================================*/
/**
 *  Main class for RayCastingRenderer.
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

} // end of namespace RayCastingRenderer

} // end of namespace kvsview
