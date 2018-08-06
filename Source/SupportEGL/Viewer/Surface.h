#pragma once
#include "../EGL.h"
#include "Display.h"
#include "Config.h"


namespace kvs
{

namespace egl
{

class Surface
{
private:
    EGLSurface m_handle;
    kvs::egl::Display& m_display;

public:
    Surface( kvs::egl::Display& display );
    ~Surface();

    void* handle() { return m_handle; }
    bool create( kvs::egl::Config& config, const EGLint* attribs );
    void destroy();
};

} // end of namespace egl

} // end of namespace kvs
