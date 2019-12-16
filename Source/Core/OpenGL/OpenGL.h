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
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/Xform>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/ValueArray>


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

void Hint( GLenum target, GLenum mode );

void SetColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
void SetDepthMask( GLboolean flag );
void SetDepthFunc( GLenum func );
void SetAlphaFunc( GLenum func, GLclampf ref );
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

void SetRasterPos( const kvs::Vec2& p );
void SetRasterPos( const kvs::Vec3& p );
void SetRasterPos( const kvs::Vec4& p );

void SetPixelStorageMode( GLenum pname, GLfloat param );
void SetPixelStorageMode( GLenum pname, GLint param );

void SetViewport( GLint x, GLint y, GLsizei width, GLsizei height );
void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far );
void SetOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top );
void SetPerspective( GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far );
void SetFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far );
void SetLookAt(
    GLdouble eyex, GLdouble eyey, GLdouble eyez,
    GLdouble centerx, GLdouble centery, GLdouble centerz,
    GLdouble upx, GLdouble upy, GLdouble upz );

void SetViewport( const kvs::Vec4& v );
void SetOrtho( const kvs::Vec4& v );
void SetOrtho( const kvs::Vec4& v, const kvs::Real32 near, const kvs::Real32 far );
void SetLookAt( const kvs::Vec3& eye, const kvs::Vec3& center, const kvs::Vec3& up );

void SetLight( GLenum light, GLenum pname, GLfloat param );
void SetLight( GLenum light, GLenum pname, GLfloat* params );

void SetLightModel( GLenum pname, GLfloat param );
void SetLightModel( GLenum pname, GLint param );

void SetClearDepth( GLdouble depth );
void SetClearColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
void SetClearColor( const kvs::Vec3& color );
void SetClearColor( const kvs::Vec4& color );
void SetClearColor( const kvs::RGBAColor& color );
void SetPolygonOffset( GLfloat factor, GLfloat units );
void SetPolygonStipple( const GLubyte* pattern );
void SetLineWidth( GLfloat width );
void SetLineStipple( GLint factor, GLushort pattern );
void SetPointSize( GLfloat size );

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

void LoadMatrix( const kvs::Xform& x );
void MultMatrix( const kvs::Xform& x );
void Rotate( GLfloat angle, const kvs::Vec3& r );
void Scale( const kvs::Vec3& s );
void Translate( const kvs::Vec3& t );

void Begin( GLenum mode );
void End();

void Vertex( GLshort x, GLshort y );
void Vertex( GLint x, GLint y );
void Vertex( GLfloat x, GLfloat y );
void Vertex( GLdouble x, GLdouble y );
void Vertex( GLshort x, GLshort y, GLshort z );
void Vertex( GLint x, GLint y, GLint z );
void Vertex( GLfloat x, GLfloat y, GLfloat z );
void Vertex( GLdouble x, GLdouble y, GLdouble z );
void Vertex( GLshort x, GLshort y, GLshort z, GLshort w );
void Vertex( GLint x, GLint y, GLint z, GLint w );
void Vertex( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
void Vertex( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
void Vertex2( const GLshort* v );
void Vertex2( const GLint* v );
void Vertex2( const GLfloat* v );
void Vertex2( const GLdouble* v );
void Vertex3( const GLshort* v );
void Vertex3( const GLint* v );
void Vertex3( const GLfloat* v );
void Vertex3( const GLdouble* v );
void Vertex4( const GLshort* v );
void Vertex4( const GLint* v );
void Vertex4( const GLfloat* v );
void Vertex4( const GLdouble* v );
void Vertex( const kvs::Vec2& v );
void Vertex( const kvs::Vec2i& v );
void Vertex( const kvs::Vec2d& v );
void Vertex( const kvs::Vec3& v );
void Vertex( const kvs::Vec3i& v );
void Vertex( const kvs::Vec3d& v );
void Vertex( const kvs::Vec4& v );
void Vertex( const kvs::Vec4i& v );
void Vertex( const kvs::Vec4d& v );
void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1 );
void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1, const kvs::Vec2& v2 );
void Vertices( const kvs::Vec2& v0, const kvs::Vec2& v1, const kvs::Vec2& v2, const kvs::Vec2& v3 );
void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1 );
void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1, const kvs::Vec3& v2 );
void Vertices( const kvs::Vec3& v0, const kvs::Vec3& v1, const kvs::Vec3& v2, const kvs::Vec3& v3 );
void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1 );
void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1, const kvs::Vec4& v2 );
void Vertices( const kvs::Vec4& v0, const kvs::Vec4& v1, const kvs::Vec4& v2, const kvs::Vec4& v3 );
void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1 );
void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1, const kvs::Vec2i& v2 );
void Vertices( const kvs::Vec2i& v0, const kvs::Vec2i& v1, const kvs::Vec2i& v2, const kvs::Vec2i& v3 );

void Color( GLbyte r, GLbyte g, GLbyte b );
void Color( GLshort r, GLshort g, GLshort b );
void Color( GLint r, GLint g, GLint b );
void Color( GLfloat r, GLfloat g, GLfloat b );
void Color( GLdouble r, GLdouble g, GLdouble b );
void Color( GLubyte r, GLubyte g, GLubyte b );
void Color( GLushort r, GLushort g, GLushort b );
void Color( GLuint r, GLuint g, GLuint b );
void Color( GLbyte r, GLbyte g, GLbyte b, GLbyte a );
void Color( GLshort r, GLshort g, GLshort b, GLshort a );
void Color( GLint r, GLint g, GLint b, GLint a );
void Color( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
void Color( GLdouble r, GLdouble g, GLdouble b, GLdouble a );
void Color( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
void Color( GLushort r, GLushort g, GLushort b, GLushort a );
void Color( GLuint r, GLuint g, GLuint b, GLuint a );
void Color( const GLbyte* c, const GLbyte a );
void Color( const GLshort* c, const GLshort a );
void Color( const GLint* c, const GLint a );
void Color( const GLfloat* c, const GLfloat a );
void Color( const GLdouble* c, const GLdouble a );
void Color( const GLubyte* c, const GLubyte a );
void Color( const GLushort* c, const GLushort a );
void Color( const GLuint* c, const GLuint a );
void Color3( const GLbyte* c );
void Color3( const GLshort* c );
void Color3( const GLint* c );
void Color3( const GLfloat* c );
void Color3( const GLdouble* c );
void Color3( const GLubyte* c );
void Color3( const GLushort* c );
void Color3( const GLuint* c );
void Color4( const GLbyte* c );
void Color4( const GLshort* c );
void Color4( const GLint* c );
void Color4( const GLfloat* c );
void Color4( const GLdouble* c );
void Color4( const GLubyte* c );
void Color4( const GLushort* c );
void Color4( const GLuint* c );
void Color( const kvs::Vec3& c );
void Color( const kvs::Vec4& c );
void Color( const kvs::RGBColor& c );
void Color( const kvs::RGBAColor& c );

void Normal( GLbyte x, GLbyte y, GLbyte z );
void Normal( GLshort x, GLshort y, GLshort z );
void Normal( GLint x, GLint y, GLint z );
void Normal( GLfloat x, GLfloat y, GLfloat z );
void Normal( GLdouble x, GLdouble y, GLdouble z );
void Normal3( const GLbyte* n );
void Normal3( const GLshort* n );
void Normal3( const GLint* n );
void Normal3( const GLfloat* n );
void Normal3( const GLdouble* n );
void Normal( const kvs::Vec3& n );

void TexCoord( GLshort s );
void TexCoord( GLint s );
void TexCoord( GLfloat s );
void TexCoord( GLdouble s );
void TexCoord( GLshort s, GLshort t );
void TexCoord( GLint s, GLint t );
void TexCoord( GLfloat s, GLfloat t );
void TexCoord( GLdouble s, GLdouble t );
void TexCoord( GLshort s, GLshort t, GLshort r );
void TexCoord( GLint s, GLint t, GLint r );
void TexCoord( GLfloat s, GLfloat t, GLfloat r );
void TexCoord( GLdouble s, GLdouble t, GLdouble r );
void TexCoord( GLshort s, GLshort t, GLshort r, GLshort q );
void TexCoord( GLint s, GLint t, GLint r, GLint q );
void TexCoord( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
void TexCoord( GLdouble s, GLdouble t, GLdouble r, GLfloat q );
void TexCoord1( const GLshort* v );
void TexCoord1( const GLint* v );
void TexCoord1( const GLfloat* v );
void TexCoord1( const GLdouble* v );
void TexCoord2( const GLshort* v );
void TexCoord2( const GLint* v );
void TexCoord2( const GLfloat* v );
void TexCoord2( const GLdouble* v );
void TexCoord3( const GLshort* v );
void TexCoord3( const GLint* v );
void TexCoord3( const GLfloat* v );
void TexCoord3( const GLdouble* v );
void TexCoord4( const GLshort* v );
void TexCoord4( const GLint* v );
void TexCoord4( const GLfloat* v );
void TexCoord4( const GLdouble* v );
void TexCoord( const kvs::Vec2& v );
void TexCoord( const kvs::Vec3& v );
void TexCoord( const kvs::Vec4& v );

void TexCoordVertex( const kvs::Vec2& tex, const kvs::Vec2& ver );

void PushMatrix();
void PopMatrix();
void PushAttrib( GLbitfield mask );
void PopAttrib();
void PushClientAttrib( GLbitfield mask );
void PopClientAttrib();

void EnableClientState( GLenum array );
void DisableClientState( GLenum array );

void EnableVertexAttribArray( GLuint index );
void DisableVertexAttribArray( GLuint index );

void VertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
void ColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
void NormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
void TexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
void VertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );

void DrawArrays( GLenum mode, GLint first, GLsizei count );
void MultiDrawArrays( GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount );
void MultiDrawArrays( GLenum mode, const kvs::ValueArray<GLint>& first, const kvs::ValueArray<GLsizei>& count );

void DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
void MultiDrawElements( GLenum mode, const GLsizei* count, GLenum type, const GLvoid* const* indices, GLsizei drawcount );
void MultiDrawElements( GLenum mode, const kvs::ValueArray<GLsizei>& count, GLenum type, const GLvoid* const* indices );

GLint Project(
    GLdouble objx, GLdouble objy, GLdouble objz,
    const GLdouble modelmat[16], const GLdouble projmat[16], const GLint viewport[4],
    GLdouble* winx, GLdouble* winy, GLdouble* winz );

GLint UnProject(
    GLdouble winx, GLdouble winy, GLdouble winz,
    const GLdouble modelmat[16], const GLdouble projmat[16], const GLint viewport[4],
    GLdouble* objx, GLdouble* objy, GLdouble* objz );

void DrawCylinder( GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks );
void DrawSphere( GLdouble radius, GLint slices, GLint stacks );

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
    void enable( GLenum cap );
    void disable( GLenum cap );
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

class Render2D
{
private:
    kvs::Vec4 m_viewport;

public:
    Render2D();
    Render2D( GLint x, GLint y, GLint width, GLint height );
    Render2D( const kvs::Vec4& viewport );
    void begin();
    void end();
    void setViewport( const kvs::Vec4& viewport ) { m_viewport = viewport; }
    void setViewport( GLint x, GLint y, GLint width, GLint height );
};

/*KVS_DEPRECATED*/ void ActivateTextureUnit( GLint unit );
/*KVS_DEPRECATED*/ inline std::string ShaderVersion() { return GLSLVersion(); }
/*KVS_DEPRECATED*/ inline bool CheckError() { return !HasError(); }

} // end of namespace OpenGL

} // end of namespace kvs

#endif // KVS__OPEN_GL_H_INCLUDE
