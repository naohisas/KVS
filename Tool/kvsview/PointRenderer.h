/*****************************************************************************/
/**
 *  @file   PointRenderer.h
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

namespace PointRenderer
{

const std::string CommandName("PointRenderer");
const std::string Description("Rendering a point object. (optional)");

/*===========================================================================*/
/**
 *  Argument class for PointRenderer.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  Main class for PointRenderer.
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

} // end of namespace PointRenderer

} // end of namespace kvsview
