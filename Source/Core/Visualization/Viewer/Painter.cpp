#include "Painter.h"
#include <kvs/OpenGL>
#include <kvs/Vector2>
#include <kvs/Matrix22>
#include <kvs/Assert>
#include <cstring>


namespace
{

inline std::string ToUTF8( int cp )
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;

    char str[8];
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return std::string( str );
}

}

namespace kvs
{

Painter::Painter():
    m_device( NULL ),
    m_device_pixel_ratio( 1.0f )
{
    memset( m_model, 0, sizeof( GLdouble ) * 16 );
    memset( m_proj, 0, sizeof( GLdouble ) * 16 );
    memset( m_view, 0, sizeof( GLint ) * 4 );
}

Painter::Painter( kvs::ScreenBase* screen )
{
    this->begin( screen );
}

Painter::~Painter()
{
    this->end();
}

bool Painter::begin( kvs::ScreenBase* screen )
{
    if ( this->isActive() ) { return false; }

    m_device = screen->paintDevice();
    m_device_pixel_ratio = screen->devicePixelRatio();

    kvs::OpenGL::GetModelViewMatrix( m_model );
    kvs::OpenGL::GetProjectionMatrix( m_proj );
    kvs::OpenGL::GetViewport( m_view );

    kvs::OpenGL::PushAttrib( GL_ALL_ATTRIB_BITS );

    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();

    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::LoadIdentity();

//    GLint view[4]; kvs::OpenGL::GetViewport( view );
//    const GLint left = view[0];
//    const GLint top = view[1];
//    const GLint right = view[0] + view[2];
//    const GLint bottom = view[1] + view[3];
    const GLint left = m_view[0];
    const GLint top = m_view[1];
    const GLint right = m_view[0] + m_view[2];
    const GLint bottom = m_view[1] + m_view[3];
    kvs::OpenGL::SetOrtho( left, right, bottom, top, 0, 1 );

    return true;
}

bool Painter::end()
{
    if ( !this->isActive() ) return false;

    kvs::OpenGL::SetMatrixMode( GL_PROJECTION );
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
    kvs::OpenGL::PopMatrix();
    kvs::OpenGL::PopAttrib();
    m_device = NULL;
    memset( m_model, 0, sizeof( GLdouble ) * 16 );
    memset( m_proj, 0, sizeof( GLdouble ) * 16 );
    memset( m_view, 0, sizeof( GLint ) * 4 );

    return true;
}

kvs::FontMetrics Painter::fontMetrics() const
{
    KVS_ASSERT( this->isActive() );
    return kvs::FontMetrics( m_font, m_device );
}

void Painter::drawText( const kvs::Vec2i& p, const std::string& text ) const
{
    KVS_ASSERT( this->isActive() );
    this->drawText( kvs::Vec2( p ), text );
}

void Painter::drawText( const kvs::Vec2& p, const std::string& text ) const
{
    KVS_ASSERT( this->isActive() );

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.disable( GL_DEPTH_TEST );
    attrib.enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    this->draw_text( p, text );
}

void Painter::drawText( const kvs::Vec3& p, const std::string& text ) const
{
    KVS_ASSERT( this->isActive() );

    GLdouble winx = 0, winy = 0, winz = 0;
    kvs::OpenGL::Project( p.x(), p.y(), p.z(), m_model, m_proj, m_view, &winx, &winy, &winz );

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.enable( GL_DEPTH_TEST );
    attrib.enable( GL_ALPHA_TEST );
    attrib.enable( GL_BLEND );
    {
        kvs::OpenGL::SetAlphaFunc( GL_GREATER, 0.0f );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        kvs::OpenGL::WithPushedMatrix modelview( GL_MODELVIEW );
        modelview.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix projection( GL_PROJECTION );
            projection.loadIdentity();
            {
                const GLint left = m_view[0];
                const GLint top = m_view[1];
                const GLint right = m_view[0] + m_view[2];
                const GLint bottom = m_view[1] + m_view[3];
                const float dpr = m_device_pixel_ratio;
                kvs::OpenGL::SetOrtho( left, right, bottom, top, 0, 1 );
                kvs::OpenGL::Translate( 0, 0, -winz );
                this->draw_text( kvs::Vec2( winx, top - ( winy - bottom ) ) / dpr, text );
            }
        }
    }
}

void Painter::drawIcon( const kvs::Vec2& p, const kvs::Font::Icon& icon, const float size ) const
{
    KVS_ASSERT( this->isActive() );

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.disable( GL_DEPTH_TEST );
    attrib.enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    kvs::FontStash* engine = m_device->textEngine();
    engine->clearState();

    const int font_id = engine->fontID( "Icon" );
    engine->setFont( font_id );

    const kvs::Vec2 d( 0.0f, engine->descender() );
    const float dpr = m_device_pixel_ratio;
    engine->setAlign( m_font.horizontalAlign() | m_font.verticalAlign() );
    engine->setColor( engine->colorID( m_font.color() ) );
    engine->setSize( size * dpr );
    engine->draw( ( p + d ) * dpr, ::ToUTF8( icon ) );
}

void Painter::draw_text( const kvs::Vec2& p, const std::string& text ) const
{
    kvs::FontStash* engine = m_device->textEngine();
    engine->clearState();

    const std::string name = m_font.fontName();
    const int font_id = engine->fontID( name );
    engine->setFont( font_id );

    const float dpr = m_device_pixel_ratio;
    const kvs::Vec2 d( 0.0f, engine->descender() );
    const kvs::Mat2 r( kvs::Mat2::Rotation( m_font.shadowAngle() ) );
    const kvs::Vec2 v( m_font.shadowDistance(), 0.0f );
    if ( m_font.isEnabledShadow() )
    {
        engine->pushState();
        engine->setAlign( m_font.horizontalAlign() | m_font.verticalAlign() );
        engine->setBlur( m_font.shadowBlur() );
        engine->setColor( engine->colorID( m_font.shadowColor() ) );
        engine->setSize( m_font.size() * m_font.shadowSizeRatio() * dpr );
        engine->draw( ( p + d + r * v ) * dpr, text );
        engine->popState();
    }

    engine->setAlign( m_font.horizontalAlign() | m_font.verticalAlign() );
    engine->setBlur( 0.0f );
    engine->setColor( engine->colorID( m_font.color() ) );
    engine->setSize( m_font.size() * dpr );
    engine->draw( ( p + d ) * dpr, text );
}

} // end of namespace kvs
