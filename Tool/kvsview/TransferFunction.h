/*****************************************************************************/
/**
 *  @file   TransferFunction.h
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
#ifndef KVSVIEW__TRANSFER_FUNCTION_H_INCLUDE
#define KVSVIEW__TRANSFER_FUNCTION_H_INCLUDE

#include <string>
#include <kvs/TransferFunction>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/CommandLine>
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

    const bool hasColorMapOption( void );

    const bool hasOpacityMapOption( void );
};

/*===========================================================================*/
/**
 *  @brief  Main class.
 */
/*===========================================================================*/
class Main
{
protected:

    int         m_argc;        ///< argument count
    char**      m_argv;        ///< argument values
    std::string m_input_name;  ///< input filename
    std::string m_output_name; ///< output filename

public:

    Main( int argc, char** argv );

public:

    const bool exec( void );
};

} // end of namespace TransferFunction

} // end of namespace kvsview

#endif // KVSVIEW__TRANSFER_FUNCTION_H_INCLUDE
