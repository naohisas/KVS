/*****************************************************************************/
/**
 *  @file   TransferFunction.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/TransferFunction>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/CommandLine>
#include <kvs/Program>
#include "Argument.h"


namespace kvsview
{

namespace TransferFunction
{

const std::string CommandName("TransferFunction");
const std::string Description("Rendering a transfer function. (optional)");

/*===========================================================================*/
/**
 *  @brief  Argument class.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:
    Argument( int argc, char** argv );
    const bool hasColorMapOption();
    const bool hasOpacityMapOption();
};

/*===========================================================================*/
/**
 *  @brief  Main class.
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
    int exec();
};

} // end of namespace TransferFunction

} // end of namespace kvsview
