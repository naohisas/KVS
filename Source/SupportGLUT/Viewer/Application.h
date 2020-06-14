/*****************************************************************************/
/**
 *  @file   Application.h
 *  @author Naohisa Sakamoto
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
    using BaseClass = kvs::ApplicationBase;

public:
    Application( int argc, char** argv );
    virtual ~Application();

    virtual int run();
    virtual void quit();
};

} // end of namespace glut

} // end of namespace kvs
