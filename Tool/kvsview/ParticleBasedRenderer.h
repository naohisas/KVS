/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParticleBasedRenderer.h 1569 2013-05-08 02:48:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVSVIEW__PARTICLE_BASED_RENDERER_H_INCLUDE
#define KVSVIEW__PARTICLE_BASED_RENDERER_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace ParticleBasedRenderer
{

const std::string CommandName("ParticleBasedRenderer");
const std::string Description("Rendering a volume object. (optional)");

/*===========================================================================*/
/**
 *  Argument class for the ParticleBasedRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:

    Argument( int argc, char** argv );

public:

    const int sampling( void ) const;

    const int shader( void ) const;

    const bool noShading( void ) const;

    const bool noLOD( void ) const;

    const bool noGPU( void ) const;

//    const bool noZooming( void ) const;

    const float ambient( void ) const;

    const float diffuse( void ) const;

    const float specular( void ) const;

    const float shininess( void ) const;

    const size_t repetitionLevel( void ) const;

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume ) const;
};

/*===========================================================================*/
/**
 *  Main class for the ParticleBasedRenderer.
 */
/*===========================================================================*/
class Main
{
protected:

    int         m_argc;         ///< argument count
    char**      m_argv;         ///< argument values
    std::string m_input_name;   ///< input filename
    std::string m_output_name;  ///< output filename

public:

    Main( int argc, char** argv );

public:

    const bool exec( void );
};

} // end of namespace ParticleBasedRenderer

} // end of namespace kvsview

#endif // KVSVIEW__PARTICLE_BASED_RENDERER_H_INCLUDE
