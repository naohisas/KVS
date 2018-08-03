/*****************************************************************************/
/**
 *  @file   SlicePlane.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SlicePlane.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
#include <kvs/PolygonObject>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace SlicePlane
{

const std::string CommandName("SlicePlane");
const std::string Description("Extract a slice plane. (optional)");

/*===========================================================================*/
/**
 *  Argument class for SlicePlane.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const kvs::Vec4 coefficients();
    const kvs::Vec3 point();
    const kvs::Vec3 normal();
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for SlicePlane.
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

} // end of namespace SlicePlane

} // end of namespace kvsview
