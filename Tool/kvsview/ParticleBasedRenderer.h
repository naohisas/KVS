/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace ParticleBasedRenderer
{

const std::string CommandName( "ParticleBasedRenderer" );
const std::string Description( "Rendering a volume object. (optional)" );

/*===========================================================================*/
/**
 *  Argument class for the ParticleBasedRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

    int sampling() const { return valueAs<int>( "sampling", 0 ); }
    int shader() const { return valueAs<int>( "shader", 0 ); }
    bool noshading() const { return hasOption( "noshading" ); }
    bool nolod() const { return hasOption( "nolod" ); }
    bool nogpu() const { return hasOption( "nogpu" ); }
    float ambient() const { return valueAs<float>( "ka", shader() == 0 ? 0.4f : 0.3f ); }
    float diffuse() const { return valueAs<float>( "kd", shader() == 0 ? 0.6f : 0.5f ); }
    float specular() const { return valueAs<float>( "ks", 0.8f ); }
    float shininess() const { return valueAs<float>( "n", 100.0f ); }
    size_t repetitionLevel() const { return valueAs<size_t>( "r", 1 ); }

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume ) const;
};

/*===========================================================================*/
/**
 *  Main class for the ParticleBasedRenderer.
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

} // end of namespace ParticleBasedRenderer

} // end of namespace kvsview
