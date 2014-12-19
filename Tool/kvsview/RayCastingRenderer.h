/*****************************************************************************/
/**
 *  @file   RayCastingRenderer.h
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
#ifndef KVSVIEW__RAY_CASTING_RENDERER_H_INCLUDE
#define KVSVIEW__RAY_CASTING_RENDERER_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
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

    const int shader( void ) const;

    const bool noShading( void ) const;

    const bool noLOD( void ) const;

    const bool noGPU( void ) const;

    const bool jittering( void ) const;

    const float ambient( void ) const;

    const float diffuse( void ) const;

    const float specular( void ) const;

    const float shininess( void ) const;

    const float step( void ) const;

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume ) const;
};

/*===========================================================================*/
/**
 *  Main class for RayCastingRenderer.
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

} // end of namespace RayCastingRenderer

} // end of namespace kvsview

#endif // KVSVIEW__RAY_CASTING_RENDERER_H_INCLUDE
