/*****************************************************************************/
/**
 *  @file   ExtractEdges.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractEdges.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/TransferFunction>
#include <kvs/Program>
#include "Argument.h"


namespace kvsview
{

namespace ExtractEdges
{

const std::string CommandName("ExtractEdges");
const std::string Description("Extract edges of the volume data. (optional)");

/*===========================================================================*/
/**
 *  Argument class for ExtractEdges.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const kvs::Real32 size();
    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for ExtractEdges.
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

} // end of namespace ExtractEdges

} // end of namespace kvsview
