#pragma once
#include "../EGL.h"
#include "Config.h"
#include "Surface.h"
#include "Display.h"


namespace kvs
{

namespace egl
{

class Context
{
private:
    EGLContext m_handle; ///< EGL rendering context
    kvs::egl::Display& m_display; ///< EGL display

public:
    Context( kvs::egl::Display& display );
    ~Context();

    void* handle() { return m_handle; }
    bool isValid() const { return m_handle != NULL; }

    bool create( kvs::egl::Config& config );
    void destroy();
    bool makeCurrent( kvs::egl::Surface& surface );
    void releaseCurrent();

    bool swapBuffers( kvs::egl::Surface& surface );
    bool swapInterval( EGLint interval );
};

} // end of namespace egl

} // end of namespace kvs
