/*****************************************************************************/
/**
 *  @file   Application.h
 *  @author Naohisa Sakamoto
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
