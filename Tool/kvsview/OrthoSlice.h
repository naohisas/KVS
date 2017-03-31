/*****************************************************************************/
/**
 *  @file   OrthoSlice.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/Program>
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
    const kvs::OrthoSlice::AlignedAxis axis();
    const float position();
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for OrthoSlice.
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

} // end of namespace OrthoSlice

} // end of namespace kvsview
