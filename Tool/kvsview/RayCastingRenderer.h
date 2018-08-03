/*****************************************************************************/
/**
 *  @file   RayCastingRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RayCastingRenderer.h 1040 2012-03-06 23:13:06Z naohisa.sakamoto@gmail.com $
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

const std::string CommandName("RayCastingRenderer");
const std::string Description("Rendering a volume object. (optional)");

/*===========================================================================*/
/**
 *  Argument class for RayCastingRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const int shader() const;
    const bool noShading() const;
    const bool noLOD() const;
    const bool noGPU() const;
    const bool jittering() const;
    const float ambient() const;
    const float diffuse() const;
    const float specular() const;
    const float shininess() const;
    const float step() const;
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume ) const;
};

/*===========================================================================*/
/**
 *  Main class for RayCastingRenderer.
 */
/*===========================================================================*/
class Main : public kvs::Program
{
private:
    std::string m_input_name;   ///< input filename
    std::string m_output_name;  ///< output filename

public:
    int exec( int argc, char** argv );
};

} // end of namespace RayCastingRenderer

} // end of namespace kvsview
