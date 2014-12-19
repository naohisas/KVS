/*****************************************************************************/
/**
 *  @file   Isosurface.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Isosurface.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__ISOSURFACE_H_INCLUDE
#define KVSVIEW__ISOSURFACE_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/PolygonObject>
#include <kvs/VolumeObjectBase>
#include <kvs/TransferFunction>
#include <kvs/glut/Slider>
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

    const kvs::PolygonObject::NormalType normalType( void );

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for Isosurface.
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

} // end of namespace Isosurface

} // end of namespace kvsview

#endif // KVSVIEW__ISOSURFACE_H_INCLUDE
