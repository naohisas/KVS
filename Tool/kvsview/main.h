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
#ifndef KVSVIEW__MAIN_H_INCLUDE
#define KVSVIEW__MAIN_H_INCLUDE


namespace kvsview
{

/*===========================================================================*/
/**
 *  Main class.
 */
/*===========================================================================*/
class Main
{
protected:

    int    m_argc; ///< argument count
    char** m_argv; ///< argument values

public:

    Main( int argc, char** argv );

public:

    bool exec( void );
};

} // end of namespace kvsview

#endif // KVSVIEW__MAIN_H_INCLUDE
