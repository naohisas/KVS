/****************************************************************************/
/**
 *  @file   OpenGL.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpenGL.h 1808 2014-08-30 00:00:29Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__OPEN_GL_H_INCLUDE
#define KVS__OPEN_GL_H_INCLUDE

#include <sstream>
#include <string>
#include <kvs/GL>
#include <kvs/StringList>
#include <kvs/Vector4>
#include <kvs/Matrix44>


namespace kvs
{

namespace OpenGL
{

std::string Description();
std::string Version();
std::string GLSLVersion();
std::string GLUVersion();
std::string GLEWVersion();
std::string Vendor();
std::string Renderer();
kvs::StringList ExtensionList();
GLenum ErrorCode();
bool HasError();
std::string ErrorString( const GLenum error_code );

void GetBooleanv( GLenum pname, GLboolean* params );
void GetDoublev( GLenum pname, GLdouble* params );
void GetFloatv( GLenum pname, GLfloat* params );
void GetIntegerv( GLenum pname, GLint* params );
GLboolean Boolean( GLenum pname );
GLdouble Double( GLenum pname );
GLfloat Float( GLenum pname );
GLint Integer( GLenum pname );

GLint MaxTextureSize();
GLint Max3DTextureSize();
GLint MaxTextureImageUnits();
GLint MaxCombinedTextureImageUnits();
GLint MaxColorAttachments();
GLint MaxRenderBufferSize();

void Flush();
void Finish();
void Clear( GLbitfield mask );

void Enable( GLenum cap );
void Disable( GLenum cap );
bool IsEnabled( GLenum cap );

void SetColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
void SetDepthMask( GLboolean flag );
void SetDepthFunc( GLenum func );
void SetBlendFunc( GLenum sfactor, GLenum dfactor );
void SetShadeModel( GLenum mode );
void SetMatrixMode( GLenum mode );
void SetPolygonMode( GLenum face, GLenum mode );
void SetColorMaterial( GLenum face, GLenum mode );
void SetCullFace( GLenum mode );
void SetReadBuffer( GLenum mode );
void SetDrawBuffer( GLenum mode );
void SetDrawBuffers( GLsizei n, const GLenum* bufs );

void SetRasterPos( GLdouble x, GLdouble y );
void SetRasterPos( GLfloat x, GLfloat y );
void SetRasterPos( GLint x, GLint y );
void SetRasterPos( GLdouble x, GLdouble y, GLdouble z );
void SetRasterPos( GLfloat x, GLfloat y, GLfloat z );
void SetRasterPos( GLint x, GLint y, GLint z );
void SetRasterPos( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
void SetRasterPos( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
void SetRasterPos( GLint x, GLint y, GLint z, GLint w );

void SetPixelStorageMode( GLenum pname, GLfloat param );
void SetPixelStorageMode( GLenum pname, GLint param );

void SetViewport( GLint x, GLint y, GLsizei width, GLsizei height );
void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far );
void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top );
void SetPerspective( GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far );
void SetFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far );

void SetLight( GLenum light, GLenum pname, GLfloat param );
void SetLight( GLenum light, GLenum pname, GLfloat* params );

void ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* data );
void DrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data );

void GetModelViewMatrix( GLfloat* params );
void GetModelViewMatrix( GLdouble* params );
void GetProjectionMatrix( GLfloat* params );
void GetProjectionMatrix( GLdouble* params );
void GetViewport( GLint* params );
void GetViewport( GLfloat* params );
kvs::Mat4 ModelViewMatrix();
kvs::Mat4 ProjectionMatrix();
kvs::Vec4 Viewport();

void LoadIdentity();
void LoadMatrix( const GLfloat* m );
void LoadMatrix( const GLdouble* m );
void MultMatrix( const GLfloat* m );
void MultMatrix( const GLdouble* m );
void Rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
void Scale( GLfloat x, GLfloat y, GLfloat z );
void Translate( GLfloat x, GLfloat y, GLfloat z );

void PushMatrix();
void PopMatrix();
void PushAttrib( GLbitfield mask );
void PopAttrib();
void PushClientAttrib( GLbitfield mask );
void PopClientAttrib();

GLint Project(
    GLdouble objx,
    GLdouble objy,
    GLdouble objz,
    const GLdouble modelmat[16],
    const GLdouble projmat[16],
    const GLint viewport[4],
    GLdouble* winx,
    GLdouble* winy,
    GLdouble* winz );

GLint UnProject(
    GLdouble winx,
    GLdouble winy,
    GLdouble winz,
    const GLdouble modelmat[16],
    const GLdouble projmat[16],
    const GLint viewport[4],
    GLdouble* objx,
    GLdouble* objy,
    GLdouble* objz );

class WithPushedMatrix
{
    GLint m_current_mode;
public:
    WithPushedMatrix( GLenum mode );
    ~WithPushedMatrix();
    void loadIdentity();
    void loadMatrix( const GLfloat* m );
    void loadMatrix( const GLdouble* m );
    void multMatrix( const GLfloat* m );
    void multMatrix( const GLdouble* m );
    void rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
    void scale( GLfloat x, GLfloat y, GLfloat z );
    void translate( GLfloat x, GLfloat y, GLfloat z );
};

class WithPushedAttrib
{
public:
    WithPushedAttrib( GLbitfield mask );
    ~WithPushedAttrib();
};

class WithPushedClientAttrib
{
public:
    WithPushedClientAttrib( GLbitfield mask );
    ~WithPushedClientAttrib();
};

class WithEnabled
{
    GLenum m_cap;
public:
    WithEnabled( GLenum cap );
    ~WithEnabled();
};

class WithDisabled
{
    GLenum m_cap;
public:
    WithDisabled( GLenum cap );
    ~WithDisabled();
};

/*KVS_DEPRECATED*/ void ActivateTextureUnit( GLint unit );
/*KVS_DEPRECATED*/ inline std::string ShaderVersion() { return GLSLVersion(); }
/*KVS_DEPRECATED*/ inline bool CheckError() { return !HasError(); }

} // end of namespace OpenGL

} // end of namespace kvs

#endif // KVS__OPEN_GL_H_INCLUDE
