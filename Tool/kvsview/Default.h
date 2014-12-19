/*****************************************************************************/
/**
 *  @file   Default.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Default.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__DEFAULT_H_INCLUDE
#define KVSVIEW__DEFAULT_H_INCLUDE

#include <string>
#include <kvs/CommandLine>
#include "Argument.h"


namespace kvsview
{

namespace Default
{

/*===========================================================================*/
/**
 *  Argument class.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:

    Argument( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  Main class.
 */
/*===========================================================================*/
class Main
{
protected:

    int         m_argc;         ///< argument count
    char**      m_argv;         ///< argument values
    std::string m_input_name;   ///< input filename
    std::string m_output_name;  ///< output filename

public:

    Main( int argc, char** argv );

public:

    const bool exec( void );
};

} // end of namespace Default

} // end of namespace kvsview

#endif // KVSVIEW__DEFAULT_H_INCLUDE
