/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.h
 *  @author Naohisa Sakamoto
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
    const int sampling() const;
    const int shader() const;
    const bool noShading() const;
    const bool noLOD() const;
    const bool noGPU() const;
//    const bool noZooming() const;
    const float ambient() const;
    const float diffuse() const;
    const float specular() const;
    const float shininess() const;
    const size_t repetitionLevel() const;
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
