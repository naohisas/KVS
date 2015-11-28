/*****************************************************************************/
/**
 *  @file   main.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#pragma once

namespace kvsconv
{

/*===========================================================================*/
/**
 *  Main class.
 */
/*===========================================================================*/
class Main
{
private:
    int m_argc; ///< argument count
    char** m_argv; ///< argument values

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    bool exec();
};

} // end of namespace kvsconv
