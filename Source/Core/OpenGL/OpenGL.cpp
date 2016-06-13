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
#if defined( GLU_VERSION )
    return ::GLUGetString( GLU_VERSION );
#else
    return "Unknown";
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
#if defined( GLEW_VERSION )
    return ::GLEWGetString( GLEW_VERSION );
#else
    return "Unknown";
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

void SetLight( GLenum light, GLenum pname, GLfloat param )
{
    KVS_GL_CALL( glLightf( light, pname, param ) );
}

void SetLight( GLenum light, GLenum pname, GLfloat* params )
{
    KVS_GL_CALL( glLightfv( light, pname, params ) );
}

void SetClearDepth( GLdouble depth )
{
    KVS_GL_CALL( glClearDepth( depth ) );
}

void SetClearDepth( GLfloat depth )
{
    KVS_GL_CALL( glClearDepthf( depth ) );
}

void SetPolygonOffset( GLfloat factor, GLfloat units )
{
    KVS_GL_CALL( glPolygonOffset( factor, units ) );
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

void ActivateTextureUnit( GLint unit )
{
    KVS_ASSERT( unit >= 0 );
    KVS_ASSERT( unit < kvs::OpenGL::MaxCombinedTextureImageUnits() );
    KVS_GL_CALL( glActiveTexture( GL_TEXTURE0 + unit ) );
}

} // end of namespace OpenGL

} // end of namespace kvs
