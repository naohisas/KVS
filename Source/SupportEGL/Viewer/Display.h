#pragma once
#include "../EGL.h"

namespace kvs
{

namespace egl
{

class Display
{
private:
    EGLDisplay m_handle; ///< handle
    EGLint m_major_version; ///< EGL major version
    EGLint m_minor_version; ///< EGL minor version

public:
    Display();
    ~Display();

    void* handle() { return m_handle; }
    EGLint majorVersion() const { return m_major_version; }
    EGLint minorVersion() const { return m_minor_version; }

    bool create( EGLNativeDisplayType display_id = EGL_DEFAULT_DISPLAY );
    bool initialize();
    void terminate();
};

} // end of namespace egl

} // end of namespace kvs
