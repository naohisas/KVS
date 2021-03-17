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
    int m_argc;
    char** m_argv;

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
//    int exec( int argc, char** argv );
    int exec();
};

} // end of namespace PolygonRenderer

} // end of namespace kvsview
