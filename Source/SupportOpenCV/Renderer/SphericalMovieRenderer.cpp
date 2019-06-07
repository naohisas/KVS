#include "SphericalMovieRenderer.h"
#include <kvs/Camera>
#include <kvs/ImageObject>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>
#include <kvs/ShaderSource>


namespace kvs
{

namespace opencv
{

void SphericalMovieRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::opencv::VideoObject* video = kvs::opencv::VideoObject::DownCast( object );
    const IplImage* frame = video->device().queryFrame();
    if ( !frame ) { return; }

    BaseClass::startTimer();

    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.disable( GL_TEXTURE_1D );
    p.disable( GL_TEXTURE_3D );
    p.enable( GL_TEXTURE_2D );

    if ( !BaseClass::texture().isCreated() ) { BaseClass::createTexture( video ); }
    if ( !m_shader_program.isCreated() ) { this->create_shader_program(); }

    const kvs::Vec2 image_size( video->width(), video->height() );
    const kvs::Vec2 screen_size( camera->windowWidth(), camera->windowHeight() );

    const int width = frame->width;
    const int height = frame->height;
    const char* data = frame->imageData; // BGRBGRBGR...

    BaseClass::texture().bind();
    BaseClass::texture().load( width, height, data );
    m_shader_program.bind();
    m_shader_program.setUniform( "spherical_map", 0 );
    m_shader_program.setUniform( "R", object->xform().rotation() );
    m_shader_program.setUniform( "image_size", image_size );
    m_shader_program.setUniform( "screen_size", screen_size );
    BaseClass::textureMapping();
    BaseClass::texture().unbind();
    m_shader_program.unbind();

    BaseClass::updateFrameIndex( video );

    BaseClass::stopTimer();
}

void SphericalMovieRenderer::create_shader_program()
{
    kvs::ShaderSource vert( "spherical_mapping.vert" );
    kvs::ShaderSource frag( "spherical_mapping.frag" );
    m_shader_program.build( vert, frag );
}

} // end of namespace opencv

} // end of namespace kvs
