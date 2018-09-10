#include "Call.h"
#include <string>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>


namespace
{

inline std::string GetErrorString( const EGLint code )
{
    std::string err_msg;
    switch ( code )
    {
    case EGL_SUCCESS: err_msg = "The last function succeeded without error."; break;
    case EGL_NOT_INITIALIZED: err_msg = "EGL is not initialized, or could not be initialized, for the specified EGL display connection."; break;
    case EGL_BAD_ACCESS: err_msg = "EGL cannot access a requested resource."; break;
    case EGL_BAD_ALLOC: err_msg = "EGL failed to allocate resources for the requested operatio."; break;
    case EGL_BAD_ATTRIBUTE: err_msg = "An unrecognized attribute or attribute value was passed in the attribute list."; break;
    case EGL_BAD_CONFIG: err_msg = "An EGLConfig argument does not name a valid EGL frame buffer configuration."; break;
    case EGL_BAD_CONTEXT: err_msg = "An EGLContext argument does not name a valid EGL rendering context."; break;
    case EGL_BAD_CURRENT_SURFACE: err_msg = "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid."; break;
    case EGL_BAD_DISPLAY: err_msg = "An EGLDisplay argument does not name a valid EGL display connection."; break;
    case EGL_BAD_MATCH: err_msg = "Arguments are inconsistent."; break;
    case EGL_BAD_NATIVE_PIXMAP: err_msg = "A NativePixmapType argument does not refer to a valid native pixmap."; break;
    case EGL_BAD_NATIVE_WINDOW: err_msg = "A NativeWindowType argument does not refer to a valid native window."; break;
    case EGL_BAD_PARAMETER: err_msg = "One or more argument values are invalid."; break;
    case EGL_BAD_SURFACE: err_msg = "An EGLSurface argument does not name a valid surface configured for GL rendering."; break;
    default : err_msg = "Unknown error."; break;
    }
    return err_msg;
}

};


namespace kvs
{

namespace egl
{

namespace internal
{

bool HasError( const char* file, const int line, const char* func, const char* command )
{
#if defined( KVS_ENABLE_OPENGL )
    EGLint error = eglGetError();
    if ( error == EGL_SUCCESS ) return false;

    // Output message tag.
    std::cerr << kvs::ColorStream::Bold
              << kvs::ColorStream::Red
              << "KVS EGL ERROR"
              << kvs::ColorStream::Reset
              << ": ";

    // Output message with an error string.
    std::string message = ::GetErrorString( error );
    std::cerr << kvs::ColorStream::Underline( kvs::ColorStream::Bold( message ) ) << std::endl;
    std::cerr << "    " << "Func: " << func << std::endl;
    std::cerr << "    " << "File: " << file << ":" << line << std::endl;
    std::cerr << "    " << "EGL Command: " << command << std::endl;

    return true;
#else
    kvs::IgnoreUnusedVariable( file );
    kvs::IgnoreUnusedVariable( line );
    kvs::IgnoreUnusedVariable( func );
    kvs::IgnoreUnusedVariable( command );
    return false;
#endif
}

} // end of namespace internal

} // end of namespace egl

} // end of namespace kvs
