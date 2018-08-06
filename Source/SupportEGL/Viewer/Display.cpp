#include "Display.h"


namespace kvs
{

namespace egl
{

Display::Display():
    m_handle( EGL_NO_DISPLAY ),
    m_major_version( 0 ),
    m_minor_version( 0 )
{
}

Display::~Display()
{
}

bool Display::create( EGLNativeDisplayType display_id )
{
    KVS_EGL_CALL( m_handle = eglGetDisplay( display_id ) );
    return m_handle != EGL_NO_DISPLAY;
}

bool Display::initialize()
{
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglInitialize( m_handle, &m_major_version, &m_minor_version ) );
    return status != EGL_FALSE;
}

void Display::terminate()
{
    KVS_EGL_CALL( eglTerminate( m_handle ) );
    m_handle = EGL_NO_DISPLAY;
}

} // end of namespace egl

} // end of namespace kvs
