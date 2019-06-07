/*****************************************************************************/
/**
 *  @file   Application.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Application.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/ApplicationBase>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  GLUT viewer application class.
 */
/*===========================================================================*/
class Application : public kvs::ApplicationBase
{
public:
    Application( int argc, char** argv );
    virtual ~Application();

    virtual int run();
    virtual void quit();
};

} // end of namespace glut

} // end of namespace kvs
