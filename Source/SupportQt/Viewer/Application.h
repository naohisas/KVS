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
 *  $Id: Application.h 1443 2013-03-18 07:00:28Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/ClassName>
#include <kvs/ApplicationBase>
#include <kvs/qt/Qt>


class QApplication;

namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Qt viewer application class.
 */
/*===========================================================================*/
class Application : public kvs::ApplicationBase
{
    kvsClassName( kvs::qt::Application );

private:
    QApplication* m_app; ///< Qt application

public:
    Application( int& argc, char** argv );
    virtual ~Application();

    virtual int run();
    virtual void quit();
};

} // end of namespace qt

} // end of namespace kvs
