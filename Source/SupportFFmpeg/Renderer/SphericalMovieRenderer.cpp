/*****************************************************************************/
/**
 *  @file   SphericalMovieRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "SphericalMovieRenderer.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] input movie object
 *  @param  camera [in] camera
 *  @param  light [in] light (not used)
 */
/*===========================================================================*/
void SphericalMovieRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    BaseClass::startTimer();

    auto* movie = kvs::ffmpeg::MovieObject::DownCast( object );
    if ( !movie ) { return; }

    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.disable( GL_TEXTURE_1D );
    p.disable( GL_TEXTURE_3D );
    p.enable( GL_TEXTURE_2D );

    if ( !BaseClass::object() ) { BaseClass::setObject( object ); }
    else { BaseClass::updateObject( object ); }

    if ( !BaseClass::texture().isValid() ) { BaseClass::createTexture( movie ); }
    if ( !m_shader_program.isCreated() ) { this->create_shader_program(); }

    kvs::Texture::Binder unit( BaseClass::texture(), 0 );
    kvs::ProgramObject::Binder shader( m_shader_program );

    const kvs::Vec2 image_size( movie->width(), movie->height() );
    const kvs::Vec2 screen_size( camera->windowWidth(), camera->windowHeight() );
    m_shader_program.setUniform( "spherical_map", 0 );
    m_shader_program.setUniform( "R", object->xform().rotation() );
    m_shader_program.setUniform( "image_size", image_size );
    m_shader_program.setUniform( "screen_size", screen_size );

    BaseClass::beginFrame( movie );
    {
        const auto width = movie->width();
        const auto height = movie->height();
        const auto buffer = movie->currentBuffer(); // RGBRGB...
        BaseClass::texture().load( width, height, buffer.data() );
        BaseClass::drawTexture();
    }
    BaseClass::endFrame( movie );

    kvs::OpenGL::SetClearDepth( 1000 );
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader programs.
 */
/*===========================================================================*/
void SphericalMovieRenderer::create_shader_program()
{
    kvs::ShaderSource vert( "spherical_mapping.vert" );
    kvs::ShaderSource frag( "spherical_mapping.frag" );
    m_shader_program.build( vert, frag );
}

} // end of namespace ffmpeg

} // end of namespace kvs
