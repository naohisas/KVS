/*****************************************************************************/
/**
 *  @file   TransferFunction.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunction.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
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

public:
    int exec( int argc, char** argv );
};

} // end of namespace TransferFunction

} // end of namespace kvsview
