#include "Config.h"


namespace kvs
{

namespace egl
{

Config::Config( kvs::egl::Display& display ):
    m_handle( NULL ),
    m_display( display )
{
}

Config::~Config()
{
}

bool Config::create( const EGLint* attribs )
{
    const size_t config_size = 128;
    EGLConfig configs[ config_size ];
    EGLint nconfigs = 0;
    EGLBoolean status = EGL_FALSE;
    KVS_EGL_CALL( status = eglGetConfigs( m_display.handle(), configs, config_size, &nconfigs ) );
    if ( status == EGL_FALSE ) { return false; }
    if ( nconfigs == 0 ) { return false; }

    KVS_EGL_CALL( status = eglChooseConfig( m_display.handle(), attribs, &m_handle, 1, &nconfigs ) );
    if ( status == EGL_FALSE ) { return false; }

    return true;
}

} // end of namespace egl

} // end of namespace kvs
