/*****************************************************************************/
/**
 *  @file   ApplicationBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ApplicationBase.h 1514 2013-04-09 07:14:05Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once

#include <kvs/Compiler>
#include <list>
#if defined( KVS_COMPILER_VC )
#pragma warning(disable:4800)
#endif


namespace kvs
{

class ScreenBase;

/*===========================================================================*/
/**
 *  @brief  ApplicationBase class.
 */
/*===========================================================================*/
class ApplicationBase
{
private:
    int m_argc; ///< argument count
    char** m_argv; ///< argument values
    std::list<kvs::ScreenBase*> m_screens; ///< list of the pointer to the screen

public:
    ApplicationBase( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    virtual ~ApplicationBase() {}

    int argc() { return m_argc; }
    char** argv() { return m_argv; }
    void attach( kvs::ScreenBase* screen );
    void detach( kvs::ScreenBase* screen );
    virtual int run() = 0;
    virtual void quit() = 0;

protected:
    std::list<kvs::ScreenBase*>& screens() { return m_screens; }
};

} // end of namespace kvs
