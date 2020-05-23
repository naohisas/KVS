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
#include <cstdlib>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a ApplicationBase class.
 *  @param  argc [in] arugment count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
ApplicationBase::ApplicationBase( int argc, char** argv ):
    m_argc( argc ),
    m_argv( argv ),
    m_color( kvs::UIColor::Instance() )
{
    const char* kvs_color_mode = std::getenv( "KVS_COLOR_MODE" );
    if ( kvs_color_mode != NULL )
    {
        auto mode = std::string( kvs_color_mode );
        if ( mode == "Dark" ) { this->setColorModeToDark(); }
        else if ( mode == "Light" ) { this->setColorModeToLight(); }
    }
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
