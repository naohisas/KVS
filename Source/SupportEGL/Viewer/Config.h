#pragma once
#include "../EGL.h"
#include "Display.h"


namespace kvs
{

namespace egl
{

class Config
{
private:
    EGLConfig m_handle;
    kvs::egl::Display& m_display;

public:
    Config( kvs::egl::Display& display );
    ~Config();

    void* handle() { return m_handle; }
    bool create( const EGLint* attribs );
};

} // end of namespace egl

} // end of namespace kvs
