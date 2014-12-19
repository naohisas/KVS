/*****************************************************************************/
/**
 *  @file   OrthoSlice.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrthoSlice.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__ORTHO_SLICE_H_INCLUDE
#define KVSVIEW__ORTHO_SLICE_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
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

    const kvs::OrthoSlice::AlignedAxis axis( void );

    const float position( void );

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for OrthoSlice.
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

} // end of namespace OrthoSlice

} // end of namespace kvsview

#endif // KVSVIEW__ORTHO_SLICE_H_INCLUDE
