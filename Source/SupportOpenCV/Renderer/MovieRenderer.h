#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/Module>
#include <kvs/opencv/MovieObject>
#include <kvs/opencv/VideoRenderer>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

namespace opencv
{

class MovieRenderer : public kvs::opencv::VideoRenderer
{
    kvsModule( kvs::opencv::MovieRenderer, Renderer );
    kvsModuleBaseClass( kvs::opencv::VideoRenderer );

private:
    bool m_enable_auto_play; ///< enabled auto-play mode
    bool m_enable_loop_play; ///< enabled loop-play mode
    bool m_enable_reverse_play; ///< enabled reverse-play mode
    int m_frame_index; ///< frame index

public:
    MovieRenderer();

    int frameIndex() const { return m_frame_index; }
    bool isEnabledAutoPlay() const { return m_enable_auto_play; }
    bool isEnabledLoopPlay() const { return m_enable_loop_play; }
    bool isEnabledReversePlay() const { return m_enable_reverse_play; }

    void setEnabledAutoPlay( const bool enable ) { m_enable_auto_play = enable; }
    void setEnabledLoopPlay( const bool enable ) { m_enable_loop_play = enable; }
    void setEnabledReversePlay( const bool enable ) { m_enable_reverse_play = enable; }
    void enableAutoPlay() { this->setEnabledAutoPlay( true ); }
    void enableLoopPlay() { this->setEnabledLoopPlay( true ); }
    void enableReversePlay() { this->setEnabledReversePlay( true ); }
    void disableAutoPlay() { this->setEnabledAutoPlay( false ); }
    void disableLoopPlay() { this->setEnabledLoopPlay( false ); }
    void disableReversePlay() { this->setEnabledReversePlay( false ); }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    void updateFrameIndex( kvs::opencv::VideoObject* video );

private:
    void increment_frame_index( kvs::opencv::VideoObject* video );
    void decrement_frame_index( kvs::opencv::VideoObject* video );
};

} // end of namespace opencv

} // end of namespace kvs
