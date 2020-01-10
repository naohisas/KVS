/*****************************************************************************/
/**
 *  @file   OpenGL.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "OpenGL.h"
#include <kvs/Assert>


namespace
{

#if !defined( KVS_ENABLE_GLU )
const char* GLErrorStrings[GL_OUT_OF_MEMORY - GL_INVALID_ENUM + 1] =
{
    "invalid enumerant",
    "invalid value",
    "invalid operation",
    "stack overflow",
    "stack underflow",
    "out of memory",
};
#endif

}

namespace
{

inline std::string GLGetString( GLenum name )
{
    std::string ret = "";
    const GLubyte* c = NULL;
    KVS_GL_CALL( c = glGetString( name ) );
    while ( *c ) ret += *c++;
    return ret;
}

#if defined( KVS_ENABLE_GLU )
inline std::string GLUGetString( GLenum name )
{
    std::string ret = "";
    const GLubyte* c = NULL;
    KVS_GL_CALL( c = gluGetString( name ) );
    while ( *c ) ret += *c++;
    return ret;
}
#endif

#if defined( KVS_ENABLE_GLEW )
inline std::string GLEWGetString( GLenum name )
{
    std::string ret = "";
    const GLubyte* c = NULL;
    KVS_GL_CALL( c = glewGetString( name ) );
    while ( *c ) ret += *c++;
    return ret;
}
#endif

inline const GLubyte* GetErrorString( GLenum error_code )
{
#if defined( KVS_ENABLE_GLU )
    const GLubyte* ret;
    KVS_GL_CALL( ret = gluErrorString( error_code ) );
    return ret;
#else
    if ( error_code == 0 )
    {
        return (const GLubyte *) "no error";
    }
    if ( ( error_code >= GL_INVALID_ENUM ) && ( error_code <= GL_OUT_OF_MEMORY ) )
    {
        return (const GLubyte *) ::GLErrorStrings[error_code - GL_INVALID_ENUM];
    }
    if ( error_code == GL_TABLE_TOO_LARGE )
    {
        return (const GLubyte *) "table too large";
    }

#ifdef GL_EXT_framebuffer_object
    if ( error_code == GL_INVALID_FRAMEBUFFER_OPERATION_EXT )
    {
       return (const GLubyte*) "invalid framebuffer operation";
    }
#endif

    return 0;
#endif
}

#if !defined( KVS_ENABLE_GLU )
inline void MakeIdentity( GLdouble m[16] )
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

inline void MakeIdentity( GLfloat m[16] )
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

inline void MultMatrices( const GLdouble a[16], const GLdouble b[16], GLdouble r[16] )
{
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            r[i*4+j] =
                a[i*4+0]*b[0*4+j] +
                a[i*4+1]*b[1*4+j] +
                a[i*4+2]*b[2*4+j] +
                a[i*4+3]*b[3*4+j];
        }
    }
}

inline void MultMatrixVec(const GLdouble matrix[16], const GLdouble in[4], GLdouble out[4])
{
    for ( int i = 0; i < 4; i++ )
    {
        out[i] =
            in[0] * matrix[0*4+i] +
            in[1] * matrix[1*4+i] +
            in[2] * matrix[2*4+i] +
            in[3] * matrix[3*4+i];
    }
}

inline int InvertMatrix( const GLdouble src[16], GLdouble inverse[16] )
{
    int swap;
    double t;
    GLdouble temp[4][4];

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            temp[i][j] = src[i*4+j];
        }
    }
    ::MakeIdentity( inverse );

    for ( int i = 0; i < 4; i++ )
    {
        /*
        ** Look for largest element in column
        */
        swap = i;
        for ( int j = i + 1; j < 4; j++ )
        {
            if ( std::fabs(temp[j][i]) > std::fabs(temp[i][i]) )
            {
                swap = j;
            }
        }

        if ( swap != i )
        {
            /*
            ** Swap rows.
            */
            for ( int k = 0; k < 4; k++ )
            {
                t = temp[i][k];
                temp[i][k] = temp[swap][k];
                temp[swap][k] = t;

                t = inverse[i*4+k];
                inverse[i*4+k] = inverse[swap*4+k];
                inverse[swap*4+k] = t;
            }
        }

        if ( temp[i][i] == 0 )
        {
            /*
            ** No non-zero pivot.  The matrix is singular, which shouldn't
            ** happen.  This means the user gave us a bad matrix.
            */
            return GL_FALSE;
        }

        t = temp[i][i];
        for ( int k = 0; k < 4; k++ )
        {
            temp[i][k] /= t;
            inverse[i*4+k] /= t;
        }

        for ( int j = 0; j < 4; j++ )
        {
            if ( j != i )
            {
                t = temp[j][i];
                for ( int k = 0; k < 4; k++ )
                {
                    temp[j][k] -= temp[i][k]*t;
                    inverse[j*4+k] -= inverse[i*4+k]*t;
                }
            }
        }
    }

    return GL_TRUE;
}

inline void Normalize( float v[3] )
{
    float r = std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

inline void Cross( float v1[3], float v2[3], float result[3] )
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
#endif

}


namespace kvs
{

namespace OpenGL
{

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL description.
 *  @return OpenGL description
 */
/*===========================================================================*/
std::string Description()
{
    const std::string description( "OpenGL - The Industry's Foundation for High Performance Graphics" );
    return description;
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL version.
 *  @return OpenGL version
 */
/*===========================================================================*/
std::string Version()
{
    return ::GLGetString( GL_VERSION );
}

/*===========================================================================*/
/**
 *  @brief  Returns GLSL (OpenGL Shading Language) version.
 *  @return GLSL version
 */
/*===========================================================================*/
std::string GLSLVersion()
{
#if defined( GL_SHADING_LANGUAGE_VERSION )
    return ::GLGetString( GL_SHADING_LANGUAGE_VERSION );
#elif defined( GL_SHADING_LANGUAGE_VERSION_ARB )
    return ::GLGetString( GL_SHADING_LANGUAGE_VERSION_ARB );
#else
    return "Unknown";
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns GLU version.
 *  @return GLU version
 */
/*===========================================================================*/
std::string GLUVersion()
{
#if defined( KVS_ENABLE_GLU )
  #if defined( GLU_VERSION )
    return ::GLUGetString( GLU_VERSION );
  #else
    return "Unknown";
  #endif
#else
    return "GLU is not enabled";
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns GLEW version.
 *  @return GLEW version
 */
/*===========================================================================*/
std::string GLEWVersion()
{
#if defined( KVS_ENABLE_GLEW )
  #if defined( GLEW_VERSION )
    return ::GLEWGetString( GLEW_VERSION );
  #else
    return "Unknown";
  #endif
#else
    return "GLEW is not available";
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns vendor information.
 *  @return vender information
 */
/*===========================================================================*/
std::string Vendor()
{
    return ::GLGetString( GL_VENDOR );
}

/*===========================================================================*/
/**
 *  @brief  Returns renderer (GPU) information.
 *  @return rendere information
 */
/*===========================================================================*/
std::string Renderer()
{
    return ::GLGetString( GL_RENDERER );
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL extension name list.
 *  @return extension name list
 */
/*===========================================================================*/
kvs::StringList ExtensionList()
{
    kvs::StringList extensions;
    std::stringstream list( ::GLGetString( GL_EXTENSIONS ) );
    std::string name;
    while ( list >> name )
    {
        extensions.push_back( name );
    }

    return extensions;
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL error code.
 *  @return error code
 */
/*===========================================================================*/
GLenum ErrorCode()
{
    GLenum error_code = GL_NO_ERROR;
    KVS_GL_CALL( error_code = glGetError() );
    return error_code;
}

/*===========================================================================*/
/**
 *  @brief  Checks OpenGL error.
 *  @return true, if no error
 */
/*===========================================================================*/
bool HasError()
{
    const GLenum error_code = kvs::OpenGL::ErrorCode();
    if ( error_code == GL_NO_ERROR ) return false;
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL error string.
 *  @param  error_code [in] OpenGL error code
 *  @return error string
 */
/*===========================================================================*/
std::string ErrorString( const GLenum error_code )
{
    std::string error_string;
    const GLubyte* c = ::GetErrorString( error_code );
    while ( *c ) error_string += *c++;
    return error_string;
}

void GetBooleanv( GLenum pname, GLboolean* params )
{
    KVS_GL_CALL( glGetBooleanv( pname, params ) );
}

void GetDoublev( GLenum pname, GLdouble* params )
{
    KVS_GL_CALL( glGetDoublev( pname, params ) );
}

void GetFloatv( GLenum pname, GLfloat* params )
{
    KVS_GL_CALL( glGetFloatv( pname, params ) );
}

void GetIntegerv( GLenum pname, GLint* params )
{
    KVS_GL_CALL( glGetIntegerv( pname, params ) );
}

GLboolean Boolean( GLenum pname )
{
    GLboolean param;
    kvs::OpenGL::GetBooleanv( pname, &param );
    return param;
}

GLdouble Double( GLenum pname )
{
    GLdouble param;
    kvs::OpenGL::GetDoublev( pname, &param );
    return param;
}

GLfloat Float( GLenum pname )
{
    GLfloat param;
    kvs::OpenGL::GetFloatv( pname, &param );
    return param;
}

GLint Integer( GLenum pname )
{
    GLint param;
    kvs::OpenGL::GetIntegerv( pname, &param );
    return param;
}

GLint MaxTextureSize()
{
    return kvs::OpenGL::Integer( GL_MAX_TEXTURE_SIZE );
}

GLint Max3DTextureSize()
{
    return kvs::OpenGL::Integer( GL_MAX_3D_TEXTURE_SIZE );
}

GLint MaxTextureImageUnits()
{
    return kvs::OpenGL::Integer( GL_MAX_TEXTURE_IMAGE_UNITS );
}

GLint MaxCombinedTextureImageUnits()
{
    return kvs::OpenGL::Integer( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS );
}

GLint MaxColorAttachments()
{
    return kvs::OpenGL::Integer( GL_MAX_COLOR_ATTACHMENTS );
}

GLint MaxRenderBufferSize()
{
    return kvs::OpenGL::Integer( GL_MAX_RENDERBUFFER_SIZE );
}

void Flush()
{
    KVS_GL_CALL( glFlush() );
}

void Finish()
{
    KVS_GL_CALL( glFinish() );
}

void Clear( GLbitfield mask )
{
    KVS_GL_CALL( glClear( mask ) );
}

void Enable( GLenum cap )
{
    KVS_GL_CALL( glEnable( cap ) );
}

void Disable( GLenum cap )
{
    KVS_GL_CALL( glDisable( cap ) );
}

bool IsEnabled( GLenum cap )
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsEnabled( cap ) );
    return result == GL_TRUE;
}

void Hint( GLenum target, GLenum mode )
{
    KVS_GL_CALL( glHint( target, mode ) );
}

void SetColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
    KVS_GL_CALL( glColorMask( red, green, blue, alpha ) );
}

void SetDepthMask( GLboolean flag )
{
    KVS_GL_CALL( glDepthMask( flag ) );
}

void SetDepthFunc( GLenum func )
{
    KVS_GL_CALL( glDepthFunc( func ) );
}

void SetAlphaFunc( GLenum func, GLclampf ref )
{
    KVS_GL_CALL( glAlphaFunc( func, ref ) );
}

void SetBlendFunc( GLenum sfactor, GLenum dfactor )
{
    KVS_GL_CALL( glBlendFunc( sfactor, dfactor ) );
}

void SetShadeModel( GLenum mode )
{
    KVS_GL_CALL( glShadeModel( mode ) );
}

void SetPolygonMode( GLenum face, GLenum mode )
{
    KVS_GL_CALL( glPolygonMode( face, mode ) );
}

void SetColorMaterial( GLenum face, GLenum mode )
{
    KVS_GL_CALL( glColorMaterial( face, mode ) );
}

void SetMatrixMode( GLenum mode )
{
    KVS_GL_CALL( glMatrixMode( mode ) );
}

void SetCullFace( GLenum mode )
{
    KVS_GL_CALL( glCullFace( mode ) );
}

void SetReadBuffer( GLenum mode )
{
    KVS_GL_CALL( glReadBuffer( mode ) );
}

void SetDrawBuffer( GLenum mode )
{
    KVS_GL_CALL( glDrawBuffer( mode ) );
}

void SetDrawBuffers( GLsizei n, const GLenum* bufs )
{
    KVS_GL_CALL( glDrawBuffers( n, bufs ) );
}

void SetRasterPos( GLdouble x, GLdouble y )
{
    KVS_GL_CALL( glRasterPos2d( x, y ) );
}

void SetRasterPos( GLfloat x, GLfloat y )
{
    KVS_GL_CALL( glRasterPos2f( x, y ) );
}

void SetRasterPos( GLint x, GLint y )
{
    KVS_GL_CALL( glRasterPos2i( x, y ) );
}

void SetRasterPos( GLdouble x, GLdouble y, GLdouble z )
{
    KVS_GL_CALL( glRasterPos3d( x, y, z ) );
}

void SetRasterPos( GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL( glRasterPos3f( x, y, z ) );
}

void SetRasterPos( GLint x, GLint y, GLint z )
{
    KVS_GL_CALL( glRasterPos3i( x, y, z ) );
}

void SetRasterPos( GLdouble x, GLdouble y, GLdouble z, GLdouble w )
{
    KVS_GL_CALL( glRasterPos4d( x, y, z, w ) );
}

void SetRasterPos( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    KVS_GL_CALL( glRasterPos4f( x, y, z, w ) );
}

void SetRasterPos( GLint x, GLint y, GLint z, GLint w )
{
    KVS_GL_CALL( glRasterPos4i( x, y, z, w ) );
}

void SetRasterPos( const kvs::Vec2& p )
{
    kvs::OpenGL::SetRasterPos( p.x(), p.y() );
}

void SetRasterPos( const kvs::Vec3& p )
{
    kvs::OpenGL::SetRasterPos( p.x(), p.y(), p.z() );
}

void SetRasterPos( const kvs::Vec4& p )
{
    kvs::OpenGL::SetRasterPos( p.x(), p.y(), p.z(), p.w() );
}

void SetPixelStorageMode( GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glPixelStoref( pname, param ) );
}

void SetPixelStorageMode( GLenum pname, GLint param )
{
    KVS_GL_CALL( glPixelStorei( pname, param ) );
}

void SetViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    KVS_GL_CALL( glViewport( x, y, width, height ) );
}

void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble front, GLdouble back )
{
    KVS_GL_CALL( glOrtho( left, right, bottom, top, front, back ) );
}

void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top )
{
#if defined( KVS_ENABLE_GLU )
    KVS_GL_CALL( gluOrtho2D( left, right, bottom, top ) );
#else
    KVS_GL_CALL( glOrtho( left, right, bottom, top, -1, 1 ) );
#endif
}

void SetPerspective( GLdouble fovy, GLdouble aspect, GLdouble front, GLdouble back )
{
#if defined( KVS_ENABLE_GLU )
    KVS_GL_CALL( gluPerspective( fovy, aspect, front, back ) );
#else
    GLdouble m[4][4];
    double radians = fovy / 2 * (3.14159265358979323846) / 180;

    double delta = back - front;
    double sine = std::sin( radians );
    if ( ( delta == 0 ) || ( sine == 0 ) || ( aspect == 0 ) )
    {
        return;
    }
    double cotangent = std::cos( radians ) / sine;

    ::MakeIdentity( &m[0][0] );
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(back + front) / delta;
    m[2][3] = -1;
    m[3][2] = -2 * front * back / delta;
    m[3][3] = 0;

    KVS_GL_CALL( glMultMatrixd( &m[0][0] ) );
#endif
}

void SetFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble front, GLdouble back )
{
    KVS_GL_CALL( glFrustum( left, right, bottom, top, front, back ) );
}

void SetLookAt(
    GLdouble eyex, GLdouble eyey, GLdouble eyez,
    GLdouble centerx, GLdouble centery, GLdouble centerz,
    GLdouble upx, GLdouble upy, GLdouble upz )
{
#if defined( KVS_ENABLE_GLU )
    KVS_GL_CALL( gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz ) );
#else
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    ::Normalize(forward);

    /* Side = forward x up */
    ::Cross(forward, up, side);
    ::Normalize(side);

    /* Recompute up as: up = side x forward */
    ::Cross(side, forward, up);

    ::MakeIdentity(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    KVS_GL_CALL( glMultMatrixf( &m[0][0] ) );
    KVS_GL_CALL( glTranslated(-eyex, -eyey, -eyez) );
#endif
}

void SetViewport( const kvs::Vec4& v )
{
    const GLint x = GLint(v.x());
    const GLint y = GLint(v.y());
    const GLsizei width = GLsizei(v.z());
    const GLsizei height = GLsizei(v.w());
    kvs::OpenGL::SetViewport( x, y, width, height );
}

void SetOrtho( const kvs::Vec4& v )
{
    const GLdouble left = v[0];
    const GLdouble right = v[1];
    const GLdouble bottom = v[2];
    const GLdouble top = v[3];
    kvs::OpenGL::SetOrtho( left, right, bottom, top );
}

void SetOrtho( const kvs::Vec4& v, const kvs::Real32 near, const kvs::Real32 far )
{
    const GLdouble left = v[0];
    const GLdouble right = v[1];
    const GLdouble bottom = v[2];
    const GLdouble top = v[3];
    const GLdouble dnear = GLdouble( near );
    const GLdouble dfar = GLdouble( far );
    kvs::OpenGL::SetOrtho( left, right, bottom, top, dnear, dfar );
}

void SetLookAt( const kvs::Vec3& eye, const kvs::Vec3& center, const kvs::Vec3& up )
{
    kvs::OpenGL::SetLookAt( eye.x(), eye.y(), eye.z(), center.x(), center.y(), center.z(), up.x(), up.y(), up.z() );
}

void SetLight( GLenum light, GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glLightf( light, pname, param ) );
}

void SetLight( GLenum light, GLenum pname, GLfloat* params )
{
    KVS_GL_CALL( glLightfv( light, pname, params ) );
}

void SetLightModel( GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glLightModelf( pname, param ) );
}

void SetLightModel( GLenum pname, GLint param )
{
    KVS_GL_CALL( glLightModeli( pname, param ) );
}

void SetClearDepth( GLdouble depth )
{
    KVS_GL_CALL( glClearDepth( depth ) );
}

void SetClearColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
    KVS_GL_CALL( glClearColor( red, green, blue, alpha ) );
}

void SetClearColor( const kvs::Vec3& color )
{
    kvs::OpenGL::SetClearColor( color[0], color[1], color[2], 1.0f );
}

void SetClearColor( const kvs::Vec4& color )
{
    kvs::OpenGL::SetClearColor( color[0], color[1], color[2], color[3] );
}

void SetClearColor( const kvs::RGBAColor& color )
{
    kvs::OpenGL::SetClearColor( color.toVec4() );
}

void SetPolygonOffset( GLfloat factor, GLfloat units )
{
    KVS_GL_CALL( glPolygonOffset( factor, units ) );
}

void SetPolygonStipple( const GLubyte* pattern )
{
    KVS_GL_CALL( glPolygonStipple( pattern ) );
}

void SetLineWidth( GLfloat width )
{
    KVS_GL_CALL( glLineWidth( width ) );
}

void SetLineStipple( GLint factor, GLushort pattern )
{
    KVS_GL_CALL( glLineStipple( factor, pattern ) );
}

void SetPointSize( GLfloat size )
{
    KVS_GL_CALL( glPointSize( size ) );
}

void ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data )
{
    KVS_GL_CALL( glReadPixels( x, y, width, height, format, type, data ) );
}

void DrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data )
{
    KVS_GL_CALL( glDrawPixels( width, height, format, type, data ) );
}

void GetModelViewMatrix( GLfloat* params )
{
    kvs::OpenGL::GetFloatv( GL_MODELVIEW_MATRIX, params );
}

void GetModelViewMatrix( GLdouble* params )
{
    kvs::OpenGL::GetDoublev( GL_MODELVIEW_MATRIX, params );
}

void GetProjectionMatrix( GLfloat* params )
{
    kvs::OpenGL::GetFloatv( GL_PROJECTION_MATRIX, params );
}

void GetProjectionMatrix( GLdouble* params )
{
    kvs::OpenGL::GetDoublev( GL_PROJECTION_MATRIX, params );
}

void GetViewport( GLint* params )
{
    kvs::OpenGL::GetIntegerv( GL_VIEWPORT, params );
}

void GetViewport( GLfloat* params )
{
    kvs::OpenGL::GetFloatv( GL_VIEWPORT, params );
}

kvs::Mat4 ModelViewMatrix()
{
    GLfloat m[16]; kvs::OpenGL::GetModelViewMatrix( m );
    return kvs::Mat4(
        m[0], m[4], m[8],  m[12],
        m[1], m[5], m[9],  m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15] );
}

kvs::Mat4 ProjectionMatrix()
{
    GLfloat p[16]; kvs::OpenGL::GetProjectionMatrix( p );
    return kvs::Mat4(
        p[0], p[4], p[8],  p[12],
        p[1], p[5], p[9],  p[13],
        p[2], p[6], p[10], p[14],
        p[3], p[7], p[11], p[15] );
}

kvs::Vec4 Viewport()
{
    GLfloat v[4]; kvs::OpenGL::GetViewport( v );
    return kvs::Vec4( v[0], v[1], v[2], v[3] );
}

void LoadIdentity()
{
    KVS_GL_CALL( glLoadIdentity() );
}

void LoadMatrix( const GLfloat* m )
{
    KVS_GL_CALL( glLoadMatrixf( m ) );
}

void LoadMatrix( const GLdouble* m )
{
    KVS_GL_CALL( glLoadMatrixd( m ) );
}

void MultMatrix( const GLfloat* m )
{
    KVS_GL_CALL( glMultMatrixf( m ) );
}

void MultMatrix( const GLdouble* m )
{
    KVS_GL_CALL( glMultMatrixd( m ) );
}

void Rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL( glRotatef( angle, x, y, z ) );
}

void Scale( GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL( glScalef( x, y, z ) );
}

void Translate( GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL( glTranslatef( x, y, z ) );
}

void LoadMatrix( const kvs::Xform& x )
{
    float m[16]; x.toArray( m );
    kvs::OpenGL::LoadMatrix( &(m[0]) );
}

void MultMatrix( const kvs::Xform& x )
{
    float m[16]; x.toArray( m );
    kvs::OpenGL::MultMatrix( &(m[0]) );
}

void Rotate( GLfloat angle, const kvs::Vec3& r )
{
    kvs::OpenGL::Rotate( angle, r.x(), r.y(), r.z() );
}

void Scale( const kvs::Vec3& s )
{
    kvs::OpenGL::Scale( s.x(), s.y(), s.z() );
}

void Translate( const kvs::Vec3& t )
{
    kvs::OpenGL::Translate( t.x(), t.y(), t.z() );
}

void Begin( GLenum mode )
{
    KVS_GL_CALL_BEG( glBegin( mode ) );
}

void End()
{
    KVS_GL_CALL_END( glEnd() );
}

void Vertex( GLshort x, GLshort y )
{
    KVS_GL_CALL_VER( glVertex2s( x, y ) );
}

void Vertex( GLint x, GLint y )
{
    KVS_GL_CALL_VER( glVertex2i( x, y ) );
}

void Vertex( GLfloat x, GLfloat y )
{
    KVS_GL_CALL_VER( glVertex2f( x, y ) );
}

void Vertex( GLdouble x, GLdouble y )
{
    KVS_GL_CALL_VER( glVertex2d( x, y ) );
}

void Vertex( GLshort x, GLshort y, GLshort z )
{
    KVS_GL_CALL_VER( glVertex3s( x, y, z ) );
}

void Vertex( GLint x, GLint y, GLint z )
{
    KVS_GL_CALL_VER( glVertex3i( x, y, z ) );
}

void Vertex( GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL_VER( glVertex3f( x, y, z ) );
}

void Vertex( GLdouble x, GLdouble y, GLdouble z )
{
    KVS_GL_CALL_VER( glVertex3d( x, y, z ) );
}

void Vertex( GLshort x, GLshort y, GLshort z, GLshort w )
{
    KVS_GL_CALL_VER( glVertex4s( x, y, z, w ) );
}

void Vertex( GLint x, GLint y, GLint z, GLint w )
{
    KVS_GL_CALL_VER( glVertex4i( x, y, z, w ) );
}

void Vertex( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    KVS_GL_CALL_VER( glVertex4f( x, y, z, w ) );
}

void Vertex( GLdouble x, GLdouble y, GLdouble z, GLdouble w )
{
    KVS_GL_CALL_VER( glVertex4d( x, y, z, w ) );
}

void Vertex2( const GLshort* v )
{
    KVS_GL_CALL_VER( glVertex2sv( v ) );
}

void Vertex2( const GLint* v )
{
    KVS_GL_CALL_VER( glVertex2iv( v ) );
}

void Vertex2( const GLfloat* v )
{
    KVS_GL_CALL_VER( glVertex2fv( v ) );
}

void Vertex2( const GLdouble* v )
{
    KVS_GL_CALL_VER( glVertex2dv( v ) );
}

void Vertex3( const GLshort* v )
{
    KVS_GL_CALL_VER( glVertex3sv( v ) );
}

void Vertex3( const GLint* v )
{
    KVS_GL_CALL_VER( glVertex3iv( v ) );
}

void Vertex3( const GLfloat* v )
{
    KVS_GL_CALL_VER( glVertex3fv( v ) );
}

void Vertex3( const GLdouble* v )
{
    KVS_GL_CALL_VER( glVertex3dv( v ) );
}

void Vertex4( const GLshort* v )
{
    KVS_GL_CALL_VER( glVertex4sv( v ) );
}

void Vertex4( const GLint* v )
{
    KVS_GL_CALL_VER( glVertex4iv( v ) );
}

void Vertex4( const GLfloat* v )
{
    KVS_GL_CALL_VER( glVertex4fv( v ) );
}

void Vertex4( const GLdouble* v )
{
    KVS_GL_CALL_VER( glVertex4dv( v ) );
}

void Vertex( const kvs::Vec2& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y() );
}

void Vertex( const kvs::Vec2i& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y() );
}

void Vertex( const kvs::Vec2d& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y() );
}

void Vertex( const kvs::Vec3& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertex( const kvs::Vec3i& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertex( const kvs::Vec3d& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertex( const kvs::Vec4& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertex( const kvs::Vec4i& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertex( const kvs::Vec4d& v )
{
    kvs::OpenGL::Vertex( v.x(), v.y(), v.z() );
}

void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
}

void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1, const kvs::Vec2& v2 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
}

void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1, const kvs::Vec2& v2, const kvs::Vec2& v3 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
    kvs::OpenGL::Vertex( v3 );
}

void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
}

void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1, const kvs::Vec2i& v2 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
}

void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1, const kvs::Vec2i& v2, const kvs::Vec2i& v3 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
    kvs::OpenGL::Vertex( v3 );
}

void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
}

void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1, const kvs::Vec3& v2 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
}

void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1, const kvs::Vec3& v2, const kvs::Vec3& v3 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
    kvs::OpenGL::Vertex( v3 );
}

void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
}

void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1, const kvs::Vec4& v2 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
}

void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1, const kvs::Vec4& v2, const kvs::Vec4& v3 )
{
    kvs::OpenGL::Vertex( v0 );
    kvs::OpenGL::Vertex( v1 );
    kvs::OpenGL::Vertex( v2 );
    kvs::OpenGL::Vertex( v3 );
}

void Color( GLbyte r, GLbyte g, GLbyte b )
{
    KVS_GL_CALL_VER( glColor3b( r, g, b ) );
}

void Color( GLshort r, GLshort g, GLshort b )
{
    KVS_GL_CALL_VER( glColor3s( r, g, b ) );
}

void Color( GLint r, GLint g, GLint b )
{
    KVS_GL_CALL_VER( glColor3i( r, g, b ) );
}

void Color( GLfloat r, GLfloat g, GLfloat b )
{
    KVS_GL_CALL_VER( glColor3f( r, g, b ) );
}

void Color( GLdouble r, GLdouble g, GLdouble b )
{
    KVS_GL_CALL_VER( glColor3d( r, g, b ) );
}

void Color( GLubyte r, GLubyte g, GLubyte b )
{
    KVS_GL_CALL_VER( glColor3ub( r, g, b ) );
}

void Color( GLushort r, GLushort g, GLushort b )
{
    KVS_GL_CALL_VER( glColor3us( r, g, b ) );
}

void Color( GLuint r, GLuint g, GLuint b )
{
    KVS_GL_CALL_VER( glColor3ui( r, g, b ) );
}

void Color( GLbyte r, GLbyte g, GLbyte b, GLbyte a )
{
    KVS_GL_CALL_VER( glColor4b( r, g, b, a ) );
}

void Color( GLshort r, GLshort g, GLshort b, GLshort a )
{
    KVS_GL_CALL_VER( glColor4s( r, g, b, a ) );
}

void Color( GLint r, GLint g, GLint b, GLint a )
{
    KVS_GL_CALL_VER( glColor4i( r, g, b, a ) );
}

void Color( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    KVS_GL_CALL_VER( glColor4f( r, g, b, a ) );
}

void Color( GLdouble r, GLdouble g, GLdouble b, GLdouble a )
{
    KVS_GL_CALL_VER( glColor4d( r, g, b, a ) );
}

void Color( GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    KVS_GL_CALL_VER( glColor4ub( r, g, b, a ) );
}

void Color( GLushort r, GLushort g, GLushort b, GLushort a )
{
    KVS_GL_CALL_VER( glColor4us( r, g, b, a ) );
}

void Color( GLuint r, GLuint g, GLuint b, GLuint a )
{
    KVS_GL_CALL_VER( glColor4ui( r, g, b, a ) );
}

void Color( const GLbyte* c, const GLbyte a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLshort* c, const GLshort a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLint* c, const GLint a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLfloat* c, const GLfloat a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLdouble* c, const GLdouble a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLubyte* c, const GLubyte a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLushort* c, const GLushort a )
{
    Color( c[0], c[1], c[2], a );
}

void Color( const GLuint* c, const GLuint a )
{
    Color( c[0], c[1], c[2], a );
}

void Color3( const GLbyte* c )
{
    KVS_GL_CALL_VER( glColor3bv( c ) );
}

void Color3( const GLshort* c )
{
    KVS_GL_CALL_VER( glColor3sv( c ) );
}

void Color3( const GLint* c )
{
    KVS_GL_CALL_VER( glColor3iv( c ) );
}

void Color3( const GLfloat* c )
{
    KVS_GL_CALL_VER( glColor3fv( c ) );
}

void Color3( const GLdouble* c )
{
    KVS_GL_CALL_VER( glColor3dv( c ) );
}

void Color3( const GLubyte* c )
{
    KVS_GL_CALL_VER( glColor3ubv( c ) );
}

void Color3( const GLushort* c )
{
    KVS_GL_CALL_VER( glColor3usv( c ) );
}

void Color3( const GLuint* c )
{
    KVS_GL_CALL_VER( glColor3uiv( c ) );
}

void Color4( const GLbyte* c )
{
    KVS_GL_CALL_VER( glColor4bv( c ) );
}

void Color4( const GLshort* c )
{
    KVS_GL_CALL_VER( glColor4sv( c ) );
}

void Color4( const GLint* c )
{
    KVS_GL_CALL_VER( glColor4iv( c ) );
}

void Color4( const GLfloat* c )
{
    KVS_GL_CALL_VER( glColor4fv( c ) );
}

void Color4( const GLdouble* c )
{
    KVS_GL_CALL_VER( glColor4dv( c ) );
}

void Color4( const GLubyte* c )
{
    KVS_GL_CALL_VER( glColor4ubv( c ) );
}

void Color4( const GLushort* c )
{
    KVS_GL_CALL_VER( glColor4usv( c ) );
}

void Color4( const GLuint* c )
{
    KVS_GL_CALL_VER( glColor4uiv( c ) );
}

void Color( const kvs::Vec3& c )
{
    kvs::OpenGL::Color( c.x(), c.y(), c.z() );
}

void Color( const kvs::Vec4& c )
{
    kvs::OpenGL::Color( c.x(), c.y(), c.z(), c.w() );
}

void Color( const kvs::RGBColor& c )
{
    kvs::OpenGL::Color( c.toVec3() );
}

void Color( const kvs::RGBAColor& c )
{
    kvs::OpenGL::Color( c.toVec4() );
}

void Normal( GLbyte x, GLbyte y, GLbyte z )
{
    KVS_GL_CALL_VER( glNormal3b( x, y, z ) );
}

void Normal( GLshort x, GLshort y, GLshort z )
{
    KVS_GL_CALL_VER( glNormal3s( x, y, z ) );
}

void Normal( GLint x, GLint y, GLint z )
{
    KVS_GL_CALL_VER( glNormal3i( x, y, z ) );
}

void Normal( GLfloat x, GLfloat y, GLfloat z )
{
    KVS_GL_CALL_VER( glNormal3f( x, y, z ) );
}

void Normal( GLdouble x, GLdouble y, GLdouble z )
{
    KVS_GL_CALL_VER( glNormal3d( x, y, z ) );
}

void Normal3( const GLbyte* n )
{
    KVS_GL_CALL_VER( glNormal3bv( n ) );
}

void Normal3( const GLshort* n )
{
    KVS_GL_CALL_VER( glNormal3sv( n ) );
}

void Normal3( const GLint* n )
{
    KVS_GL_CALL_VER( glNormal3iv( n ) );
}

void Normal3( const GLfloat* n )
{
    KVS_GL_CALL_VER( glNormal3fv( n ) );
}

void Normal( const kvs::Vec3& n )
{
    kvs::OpenGL::Normal( n.x(), n.y(), n.z() );
}

void Normal3( const GLdouble* n )
{
    KVS_GL_CALL_VER( glNormal3dv( n ) );
}

void TexCoord( GLshort s )
{
    KVS_GL_CALL_VER( glTexCoord1s( s ) );
}

void TexCoord( GLint s )
{
    KVS_GL_CALL_VER( glTexCoord1i( s ) );
}

void TexCoord( GLfloat s )
{
    KVS_GL_CALL_VER( glTexCoord1f( s ) );
}

void TexCoord( GLdouble s )
{
    KVS_GL_CALL_VER( glTexCoord1d( s ) );
}

void TexCoord( GLshort s, GLshort t )
{
    KVS_GL_CALL_VER( glTexCoord2s( s, t ) );
}

void TexCoord( GLint s, GLint t )
{
    KVS_GL_CALL_VER( glTexCoord2i( s, t ) );
}

void TexCoord( GLfloat s, GLfloat t )
{
    KVS_GL_CALL_VER( glTexCoord2f( s, t ) );
}

void TexCoord( GLdouble s, GLdouble t )
{
    KVS_GL_CALL_VER( glTexCoord2d( s, t ) );
}

void TexCoord( GLshort s, GLshort t, GLshort r )
{
    KVS_GL_CALL_VER( glTexCoord3s( s, t, r ) );
}

void TexCoord( GLint s, GLint t, GLint r )
{
    KVS_GL_CALL_VER( glTexCoord3i( s, t, r ) );
}

void TexCoord( GLfloat s, GLfloat t, GLfloat r )
{
    KVS_GL_CALL_VER( glTexCoord3f( s, t, r ) );
}

void TexCoord( GLdouble s, GLdouble t, GLdouble r )
{
    KVS_GL_CALL_VER( glTexCoord3d( s, t, r ) );
}

void TexCoord( GLshort s, GLshort t, GLshort r, GLshort q )
{
    KVS_GL_CALL_VER( glTexCoord4s( s, t, r, q ) );
}

void TexCoord( GLint s, GLint t, GLint r, GLint q )
{
    KVS_GL_CALL_VER( glTexCoord4i( s, t, r, q ) );
}

void TexCoord( GLfloat s, GLfloat t, GLfloat r, GLfloat q )
{
    KVS_GL_CALL_VER( glTexCoord4f( s, t, r, q ) );
}

void TexCoord( GLdouble s, GLdouble t, GLdouble r, GLfloat q )
{
    KVS_GL_CALL_VER( glTexCoord4d( s, t, r, q ) );
}

void TexCoord1( const GLshort* v )
{
    KVS_GL_CALL_VER( glTexCoord1sv( v ) );
}

void TexCoord1( const GLint* v )
{
    KVS_GL_CALL_VER( glTexCoord1iv( v ) );
}

void TexCoord1( const GLfloat* v )
{
    KVS_GL_CALL_VER( glTexCoord1fv( v ) );
}

void TexCoord1( const GLdouble* v )
{
    KVS_GL_CALL_VER( glTexCoord1dv( v ) );
}

void TexCoord2( const GLshort* v )
{
    KVS_GL_CALL_VER( glTexCoord2sv( v ) );
}

void TexCoord2( const GLint* v )
{
    KVS_GL_CALL_VER( glTexCoord2iv( v ) );
}

void TexCoord2( const GLfloat* v )
{
    KVS_GL_CALL_VER( glTexCoord2fv( v ) );
}

void TexCoord2( const GLdouble* v )
{
    KVS_GL_CALL_VER( glTexCoord2dv( v ) );
}

void TexCoord3( const GLshort* v )
{
    KVS_GL_CALL_VER( glTexCoord3sv( v ) );
}

void TexCoord3( const GLint* v )
{
    KVS_GL_CALL_VER( glTexCoord3iv( v ) );
}

void TexCoord3( const GLfloat* v )
{
    KVS_GL_CALL_VER( glTexCoord3fv( v ) );
}

void TexCoord3( const GLdouble* v )
{
    KVS_GL_CALL_VER( glTexCoord3dv( v ) );
}

void TexCoord4( const GLshort* v )
{
    KVS_GL_CALL_VER( glTexCoord4sv( v ) );
}

void TexCoord4( const GLint* v )
{
    KVS_GL_CALL_VER( glTexCoord4iv( v ) );
}

void TexCoord4( const GLfloat* v )
{
    KVS_GL_CALL_VER( glTexCoord4fv( v ) );
}

void TexCoord4( const GLdouble* v )
{
    KVS_GL_CALL_VER( glTexCoord4dv( v ) );
}

void TexCoord( const kvs::Vec2& v )
{
    kvs::OpenGL::TexCoord( v[0], v[1] );
}

void TexCoord( const kvs::Vec3& v )
{
    kvs::OpenGL::TexCoord( v[0], v[1], v[2] );
}

void TexCoord( const kvs::Vec4& v )
{
    kvs::OpenGL::TexCoord( v[0], v[1], v[2], v[3] );
}

void TexCoordVertex( const kvs::Vec2& tex, const kvs::Vec2& ver )
{
    kvs::OpenGL::TexCoord( tex );
    kvs::OpenGL::Vertex( ver );
}

void PushMatrix()
{
    KVS_GL_CALL( glPushMatrix() );
}

void PopMatrix()
{
    KVS_GL_CALL( glPopMatrix() );
}

void PushAttrib( GLbitfield mask )
{
    KVS_GL_CALL( glPushAttrib( mask ) );
}

void PopAttrib()
{
    KVS_GL_CALL( glPopAttrib() );
}

void PushClientAttrib( GLbitfield mask )
{
    KVS_GL_CALL( glPushClientAttrib( mask ) );
}

void PopClientAttrib()
{
    KVS_GL_CALL( glPopClientAttrib() );
}

void EnableClientState( GLenum array )
{
    KVS_GL_CALL( glEnableClientState( array ) );
}

void DisableClientState( GLenum array )
{
    KVS_GL_CALL( glDisableClientState( array ) );
}

void EnableVertexAttribArray( GLuint index )
{
    KVS_GL_CALL( glEnableVertexAttribArray( index ) );
}

void DisableVertexAttribArray( GLuint index )
{
    KVS_GL_CALL( glDisableVertexAttribArray( index ) );
}

void VertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    KVS_GL_CALL( glVertexPointer( size, type, stride, pointer ) );
}

void ColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    KVS_GL_CALL( glColorPointer( size, type, stride, pointer ) );
}

void NormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer )
{
    KVS_GL_CALL( glNormalPointer( type, stride, pointer ) );
}

void TexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
{
    KVS_GL_CALL( glTexCoordPointer( size, type, stride, pointer ) );
}

void VertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer )
{
    KVS_GL_CALL( glVertexAttribPointer( index, size, type, normalized, stride, pointer ) );
}

void DrawArrays( GLenum mode, GLint first, GLsizei count )
{
    KVS_GL_CALL( glDrawArrays( mode, first, count ) );
}

void MultiDrawArrays( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount )
{
// if OpenGL version is 1.4 or later?
    KVS_GL_CALL( glMultiDrawArrays( mode, first, count, drawcount ) );
// else
//    for ( GLsizei i = 0; i < drawcount; ++i )
//    {
//        if ( count[i] > 0 ) { kvs::OpenGL::DrawArrays( mode, first[i], count[i] ); }
//    }
}

void MultiDrawArrays( GLenum mode, const kvs::ValueArray<GLint>& first, const kvs::ValueArray<GLsizei>& count )
{
    kvs::OpenGL::MultiDrawArrays( mode, first.data(), count.data(), first.size() );
}

void DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    KVS_GL_CALL( glDrawElements( mode, count, type, indices ) );
}

void MultiDrawElements( GLenum mode, const GLsizei* count, GLenum type, const GLvoid* const* indices, GLsizei drawcount )
{
// if OpenGL version is 1.4 or later?
    KVS_GL_CALL( glMultiDrawElements( mode, count, type, (const GLvoid**)indices, drawcount ) );
// else
//    for ( GLsizei i = 0; i < drawcount; ++i )
//    {
//        if ( count[i] > 0 ) { kvs::OpenGL::DrawElements( mode, count[i], type, indices ); }
//    }
}

void MultiDrawElements( GLenum mode, const kvs::ValueArray<GLsizei>& count, GLenum type, const GLvoid* const* indices )
{
    kvs::OpenGL::MultiDrawElements( mode, count.data(), type, indices, count.size() );
}

GLint Project(
    GLdouble objx,
    GLdouble objy,
    GLdouble objz,
    const GLdouble modelmat[16],
    const GLdouble projmat[16],
    const GLint viewport[4],
    GLdouble* winx,
    GLdouble* winy,
    GLdouble* winz )
{
#if defined( KVS_ENABLE_GLU )
    GLint ret = 0;
    KVS_GL_CALL( ret = gluProject( objx, objy, objz, modelmat, projmat, viewport, winx, winy, winz ) );
    return ret;
#else
    double in[4];
    double out[4];

    in[0]=objx;
    in[1]=objy;
    in[2]=objz;
    in[3]=1.0;

    ::MultMatrixVec( modelmat, in, out );
    ::MultMatrixVec( projmat, out, in );
    if ( in[3] == 0.0 ) { return GL_FALSE; }

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    /* Map x, y and z to range 0-1 */
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;

    /* Map x,y to viewport */
    in[0] = in[0] * viewport[2] + viewport[0];
    in[1] = in[1] * viewport[3] + viewport[1];

    *winx=in[0];
    *winy=in[1];
    *winz=in[2];

    return GL_TRUE;
#endif
}

GLint UnProject(
    GLdouble winx,
    GLdouble winy,
    GLdouble winz,
    const GLdouble modelmat[16],
    const GLdouble projmat[16],
    const GLint viewport[4],
    GLdouble* objx,
    GLdouble* objy,
    GLdouble* objz )
{
#if defined( KVS_ENABLE_GLU )
    GLint ret = 0;
    KVS_GL_CALL( ret = gluUnProject( winx, winy, winz, modelmat, projmat, viewport, objx, objy, objz ) );
    return ret;
#else
    double finalmat[16];
    double in[4];
    double out[4];

    ::MultMatrices( modelmat, projmat, finalmat );
    if ( !::InvertMatrix( finalmat, finalmat ) ) { return GL_FALSE; }

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    /* Map x and y from window coordinates */
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    ::MultMatrixVec( finalmat, in, out );
    if ( out[3] == 0.0 ) { return GL_FALSE; }
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];

    return GL_TRUE;
#endif
}

void DrawCylinder( GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks )
{
    const int CacheSize = 240;
    const float Pi = 3.14159265358979323846;

    GLint i,j;
    GLfloat sinCache[CacheSize];
    GLfloat cosCache[CacheSize];
    GLfloat sinCache2[CacheSize];
    GLfloat cosCache2[CacheSize];
    GLfloat sinCache3[CacheSize];
    GLfloat cosCache3[CacheSize];
    GLfloat angle;
    GLfloat zLow, zHigh;
    GLfloat length;
    GLfloat deltaRadius;
    GLfloat zNormal;
    GLfloat xyNormalRatio;
    GLfloat radiusLow, radiusHigh;
    int needCache2, needCache3;

    if (slices >= CacheSize) slices = CacheSize-1;

    if (slices < 2 || stacks < 1 || base < 0.0 || top < 0.0 || height < 0.0)
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Compute length (needed for normal calculations) */
    deltaRadius = base - top;
    length = std::sqrt(deltaRadius*deltaRadius + height*height);
    if ( length == 0.0 )
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = 1;
    needCache3 = 0;

    zNormal = deltaRadius / length;
    xyNormalRatio = height / length;

    for (i = 0; i < slices; i++)
    {
        angle = 2 * Pi * i / slices;
        if (needCache2)
        {
            sinCache2[i] = xyNormalRatio * std::sin(angle);
            cosCache2[i] = xyNormalRatio * std::cos(angle);
        }
        sinCache[i] = std::sin(angle);
        cosCache[i] = std::cos(angle);
    }

    if (needCache3)
    {
        for (i = 0; i < slices; i++)
        {
            angle = 2 * Pi * (i-0.5) / slices;
            sinCache3[i] = xyNormalRatio * std::sin(angle);
            cosCache3[i] = xyNormalRatio * std::cos(angle);
        }
    }

    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    if (needCache2)
    {
        sinCache2[slices] = sinCache2[0];
        cosCache2[slices] = cosCache2[0];
    }
    if (needCache3)
    {
        sinCache3[slices] = sinCache3[0];
        cosCache3[slices] = cosCache3[0];
    }

    /* Note:
    ** An argument could be made for using a TRIANGLE_FAN for the end
    ** of the cylinder of either radii is 0.0 (a cone).  However, a
    ** TRIANGLE_FAN would not work in smooth shading mode (the common
    ** case) because the normal for the apex is different for every
    ** triangle (and TRIANGLE_FAN doesn't let me respecify that normal).
    ** Now, my choice is GL_TRIANGLES, or leave the GL_QUAD_STRIP and
    ** just let the GL trivially reject one of the two triangles of the
    ** QUAD.  GL_QUAD_STRIP is probably faster, so I will leave this code
    ** alone.
    */
    for (j = 0; j < stacks; j++)
    {
        zLow = j * height / stacks;
        zHigh = (j + 1) * height / stacks;
        radiusLow = base - deltaRadius * ((float) j / stacks);
        radiusHigh = base - deltaRadius * ((float) (j + 1) / stacks);

        kvs::OpenGL::Begin( GL_QUAD_STRIP );
        for (i = 0; i <= slices; i++)
        {
            kvs::OpenGL::Normal( sinCache2[i], cosCache2[i], zNormal );
            kvs::OpenGL::Vertex( radiusLow  * sinCache[i], radiusLow  * cosCache[i], zLow );
            kvs::OpenGL::Vertex( radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh );
        }
        kvs::OpenGL::End();
    }
}

void DrawSphere( GLdouble radius, GLint slices, GLint stacks )
{
    const int CacheSize = 240;
    const float Pi = 3.14159265358979323846;

    GLint i,j;
    GLfloat sinCache1a[CacheSize];
    GLfloat cosCache1a[CacheSize];
    GLfloat sinCache2a[CacheSize];
    GLfloat cosCache2a[CacheSize];
    GLfloat sinCache3a[CacheSize];
    GLfloat cosCache3a[CacheSize];
    GLfloat sinCache1b[CacheSize];
    GLfloat cosCache1b[CacheSize];
    GLfloat sinCache2b[CacheSize];
    GLfloat cosCache2b[CacheSize];
//    GLfloat sinCache3b[CacheSize];
//    GLfloat cosCache3b[CacheSize];
    GLfloat angle;
    GLfloat zLow, zHigh;
    GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
    GLfloat costemp3 = 0.0, costemp4 = 0.0;
    GLboolean needCache2, needCache3;
    GLint start, finish;

    if (slices >= CacheSize) slices = CacheSize-1;
    if (stacks >= CacheSize) stacks = CacheSize-1;
    if (slices < 2 || stacks < 1 || radius < 0.0)
    {
        kvsMessageError("Invalid value.");
        return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = GL_TRUE;
    needCache3 = GL_FALSE;

    for (i = 0; i < slices; i++)
    {
        angle = 2 * Pi * i / slices;
        sinCache1a[i] = std::sin(angle);
        cosCache1a[i] = std::cos(angle);
        if (needCache2)
        {
            sinCache2a[i] = sinCache1a[i];
            cosCache2a[i] = cosCache1a[i];
        }
    }

    for (j = 0; j <= stacks; j++)
    {
        angle = Pi * j / stacks;
        if (needCache2)
        {
            sinCache2b[j] = std::sin(angle);
            cosCache2b[j] = std::cos(angle);
        }
        sinCache1b[j] = radius * std::sin(angle);
        cosCache1b[j] = radius * std::cos(angle);
    }
    /* Make sure it comes to a point */
    sinCache1b[0] = 0;
    sinCache1b[stacks] = 0;

    if (needCache3) {
        for (i = 0; i < slices; i++)
        {
            angle = 2 * Pi * (i-0.5) / slices;
            sinCache3a[i] = std::sin(angle);
            cosCache3a[i] = std::cos(angle);
        }
//        for (j = 0; j <= stacks; j++)
//        {
//            angle = Pi * (j - 0.5) / stacks;
//            sinCache3b[j] = std::sin(angle);
//            cosCache3b[j] = std::cos(angle);
//        }
    }

    sinCache1a[slices] = sinCache1a[0];
    cosCache1a[slices] = cosCache1a[0];
    if (needCache2)
    {
        sinCache2a[slices] = sinCache2a[0];
        cosCache2a[slices] = cosCache2a[0];
    }
    if (needCache3)
    {
        sinCache3a[slices] = sinCache3a[0];
        cosCache3a[slices] = cosCache3a[0];
    }

    /* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
    ** We don't do it when texturing because we need to respecify the
    ** texture coordinates of the apex for every adjacent vertex (because
    ** it isn't a constant for that point)
    */
    start = 1;
    finish = stacks - 1;

    /* Low end first (j == 0 iteration) */
    sintemp2 = sinCache1b[1];
    zHigh = cosCache1b[1];
    sintemp3 = sinCache2b[1];
    costemp3 = cosCache2b[1];

    kvs::OpenGL::Begin(GL_TRIANGLE_FAN);
    kvs::OpenGL::Normal(sinCache2a[0] * sinCache2b[0], cosCache2a[0] * sinCache2b[0], cosCache2b[0]);
    kvs::OpenGL::Vertex(0.0, 0.0, radius);
    for (i = slices; i >= 0; i--)
    {
        kvs::OpenGL::Normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
        kvs::OpenGL::Vertex(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
    }
    kvs::OpenGL::End();

    /* High end next (j == stacks-1 iteration) */
    sintemp2 = sinCache1b[stacks-1];
    zHigh = cosCache1b[stacks-1];
    sintemp3 = sinCache2b[stacks-1];
    costemp3 = cosCache2b[stacks-1];

    kvs::OpenGL::Begin(GL_TRIANGLE_FAN);
    kvs::OpenGL::Normal(sinCache2a[stacks] * sinCache2b[stacks], cosCache2a[stacks] * sinCache2b[stacks], cosCache2b[stacks]);
    kvs::OpenGL::Vertex(0.0, 0.0, -radius);
    for (i = 0; i <= slices; i++)
    {
        kvs::OpenGL::Normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
        kvs::OpenGL::Normal(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
    }
    kvs::OpenGL::End();

    for (j = start; j < finish; j++)
    {
        zLow = cosCache1b[j];
        zHigh = cosCache1b[j+1];
        sintemp1 = sinCache1b[j];
        sintemp2 = sinCache1b[j+1];
        sintemp3 = sinCache2b[j+1];
        costemp3 = cosCache2b[j+1];
        sintemp4 = sinCache2b[j];
        costemp4 = cosCache2b[j];

        kvs::OpenGL::Begin(GL_QUAD_STRIP);
        for (i = 0; i <= slices; i++)
        {
            kvs::OpenGL::Normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
            kvs::OpenGL::Vertex(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);

            kvs::OpenGL::Normal(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
            kvs::OpenGL::Vertex(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
        }
        kvs::OpenGL::End();
    }
}

WithPushedMatrix::WithPushedMatrix( GLenum mode )
{
    m_current_mode = kvs::OpenGL::Integer( GL_MATRIX_MODE );
    kvs::OpenGL::SetMatrixMode( mode );
    kvs::OpenGL::PushMatrix();
}

WithPushedMatrix::~WithPushedMatrix()
{
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::SetMatrixMode( m_current_mode );
}

void WithPushedMatrix::loadIdentity()
{
    kvs::OpenGL::LoadIdentity();
}

void WithPushedMatrix::loadMatrix( const GLfloat* m )
{
    kvs::OpenGL::LoadMatrix( m );
}

void WithPushedMatrix::loadMatrix( const GLdouble* m )
{
    kvs::OpenGL::LoadMatrix( m );
}

void WithPushedMatrix::multMatrix( const GLfloat* m )
{
    kvs::OpenGL::MultMatrix( m );
}

void WithPushedMatrix::multMatrix( const GLdouble* m )
{
    kvs::OpenGL::MultMatrix( m );
}

void WithPushedMatrix::rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    kvs::OpenGL::Rotate( angle, x, y, z );
}

void WithPushedMatrix::scale( GLfloat x, GLfloat y, GLfloat z )
{
    kvs::OpenGL::Scale( x, y, z );
}

void WithPushedMatrix::translate( GLfloat x, GLfloat y, GLfloat z )
{
    kvs::OpenGL::Translate( x, y, z );
}

WithPushedAttrib::WithPushedAttrib( GLbitfield mask )
{
    kvs::OpenGL::PushAttrib( mask );
}

WithPushedAttrib::~WithPushedAttrib()
{
    kvs::OpenGL::PopAttrib();
}

void WithPushedAttrib::enable( GLenum cap )
{
    kvs::OpenGL::Enable( cap );
}

void WithPushedAttrib::disable( GLenum cap )
{
    kvs::OpenGL::Disable( cap );
}

WithPushedClientAttrib::WithPushedClientAttrib( GLbitfield mask )
{
    kvs::OpenGL::PushClientAttrib( mask );
}

WithPushedClientAttrib::~WithPushedClientAttrib()
{
    kvs::OpenGL::PopClientAttrib();
}

WithEnabled::WithEnabled( GLenum cap ):
    m_cap( cap )
{
    kvs::OpenGL::Enable( cap );
}

WithEnabled::~WithEnabled()
{
    kvs::OpenGL::Disable( m_cap );
}

WithDisabled::WithDisabled( GLenum cap ):
    m_cap( cap )
{
    kvs::OpenGL::Disable( cap );
}

WithDisabled::~WithDisabled()
{
    kvs::OpenGL::Enable( m_cap );
}

Render2D::Render2D()
{
}

Render2D::Render2D( GLint x, GLint y, GLint width, GLint height )
{
    this->setViewport( x, y, width, height );
}

Render2D::Render2D( const kvs::Vec4& viewport )
{
    this->setViewport( viewport );
}

void Render2D::begin()
{
    kvs::OpenGL::PushAttrib( GL_ALL_ATTRIB_BITS );

    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();

    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();

    // The origin is upper-left.
    const GLint left = m_viewport[0];
    const GLint top = m_viewport[1];
    const GLint right = m_viewport[0] + m_viewport[2];
    const GLint bottom = m_viewport[1] + m_viewport[3];
    kvs::OpenGL::SetOrtho( left, right, bottom, top, -1, 1 );
    kvs::OpenGL::Disable( GL_DEPTH_TEST );
}

void Render2D::end()
{
    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::PopMatrix();

    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::PopMatrix();

    kvs::OpenGL::PopAttrib();
}

void Render2D::setViewport( GLint x, GLint y, GLint width, GLint height )
{
    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;
}

void ActivateTextureUnit( GLint unit )
{
    KVS_ASSERT( unit >= 0 );
    KVS_ASSERT( unit < kvs::OpenGL::MaxCombinedTextureImageUnits() );
    KVS_GL_CALL( glActiveTexture( GL_TEXTURE0 + unit ) );
}

} // end of namespace OpenGL

} // end of namespace kvs
