#pragma once
#include "../OSMesa.h"
#include "Surface.h"
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

namespace osmesa
{

class Context
{
public:
    static void SetPixelStore( GLint pname, GLint value );
    static void SetRowLength( GLint value );
    static void SetYAxisDirection( GLint value );
    static void SetYAxisDirectionToDown();
    static void SetYAxisDirectionToUp();

    static void GetIntegerv( GLint pname, GLint* value );
    static GLint GetWidth();
    static GLint GetHeight();
    static GLint GetFormat();
    static GLint GetType();
    static GLint GetRowLength();
    static GLint GetYAxisDirection();

private:
    OSMesaContext m_handle;

public:
    Context();
    ~Context();

    void* handle() { return m_handle; }
    bool isValid() const { return m_handle != NULL; }

    bool create( GLenum format, GLint depth_bits, GLint stencil_bits, GLint accum_bits );
    void destroy();
    bool makeCurrent( kvs::osmesa::Surface& surface );
    void releaseCurrent();
};

} // end of namespace osmesa

} // end of namespace kvs
