/*****************************************************************************/
/**
 *  @file   PolygonRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderer.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/Program>
#include "Argument.h"


namespace kvsview
{

namespace PolygonRenderer
{

const std::string CommandName("PolygonRenderer");
const std::string Description("Rendering a polygon object. (optional)");

/*===========================================================================*/
/**
 *  Argument class for PolygonRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );

public:
    const int twoSideLighting() const;
};

/*===========================================================================*/
/**
 *  Main class for PolygonRenderer.
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

} // end of namespace PolygonRenderer

} // end of namespace kvsview
