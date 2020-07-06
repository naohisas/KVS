/*****************************************************************************/
/**
 *  @file   PolygonRenderer.h
 *  @author Naohisa Sakamoto
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
    const int twoSideLighting() const { return valueAs<int>( "t", 0 ); }
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
