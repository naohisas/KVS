/*****************************************************************************/
/**
 *  @file   MovieRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <SupportFFmpeg/Object/MovieObject.h>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Movie renderer class.
 */
/*===========================================================================*/
class MovieRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ffmpeg::MovieRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    double m_initial_aspect_ratio = 1.0; ///< initial aspect ratio
    double m_left = 0.0; ///< screen left position
    double m_right = 1.0; ///< screen right position
    double m_bottom = 0.0; ///< screen bottom position
    double m_top = 1.0; ///< screen top position
    bool m_is_centering_enabled = true; ///< if true, center alignment is enabled
    bool m_is_mirroring_enabled = false; ///< if true, mirror mapping is enabled
    bool m_playing = false; ///< if true, a movie is playing
    bool m_paused = false; ///< if true, a movie is paused
    bool m_reset_flag = false; ///< if true, frame index will be set to zero
    kvs::Texture2D m_texture{}; ///< texture image
    const kvs::ObjectBase* m_object = nullptr; ///< rendering object (reference)

public:
    MovieRenderer() = default;

    bool isCenteringEnabled() const { return m_is_centering_enabled; }
    bool isMirroringEnabled() const { return m_is_mirroring_enabled; }
    void setCenteringEnabled( const bool enable = true ) { m_is_centering_enabled = enable; }
    void setMirroringEnabled( const bool enable = true ) { m_is_mirroring_enabled = enable; }

    bool isPlaying() const { return m_playing && !m_paused; }
    bool isPaused() const { return m_paused; }
    bool isStopped() const { return !m_playing && !m_paused; }

    void play() { if ( isStopped() ) { m_reset_flag = true; } m_playing = true; m_paused = false; }
    void pause() { m_playing = true; m_paused = true; }
    void stop() { m_playing = false; m_paused = false; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Texture2D& texture() { return m_texture; }
    const kvs::ObjectBase* object() const { return m_object; }
    void setObject( const kvs::ObjectBase* object ) { m_object = object; }
    void updateObject( const kvs::ObjectBase* object );
    void createTexture( const kvs::ffmpeg::MovieObject* object );
    void alignCenter( const kvs::Camera* camera );
    void drawTexture();
    void beginFrame( kvs::ffmpeg::MovieObject* object );
    void endFrame( kvs::ffmpeg::MovieObject* object );
};

} // end of namespace ffmpeg

} // end of namespace kvs
