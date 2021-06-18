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

namespace glfw
{

/*===========================================================================*/
/**
 *  @brief  Application class.
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

private:
    void main_loop();
    bool should_close();
};

} // end of namespace glfw

} // end of namespace kvs
