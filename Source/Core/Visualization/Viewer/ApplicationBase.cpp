/*****************************************************************************/
/**
 *  @file   ApplicationBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ApplicationBase.cpp 1514 2013-04-09 07:14:05Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ApplicationBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ApplicationBase class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
ApplicationBase::ApplicationBase( int argc, char** argv ):
    m_argc( argc ),
    m_argv( argv )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ApplicationBase class.
 */
/*===========================================================================*/
ApplicationBase::~ApplicationBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the argument count.
 *  @return argument count
 */
/*===========================================================================*/
int ApplicationBase::argc()
{
    return m_argc;
}

/*===========================================================================*/
/**
 *  @brief  Returns the argument values.
 *  @return argument values
 */
/*===========================================================================*/
char** ApplicationBase::argv()
{
    return m_argv;
}

/*===========================================================================*/
/**
 *  @brief  Attaches a pointer to the screeen.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
void ApplicationBase::attach( kvs::ScreenBase* screen )
{
    m_screens.push_back( screen );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the pointer to the screen.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
void ApplicationBase::detach( kvs::ScreenBase* screen )
{
    m_screens.remove( screen );
}

} // end of namespace kvs
