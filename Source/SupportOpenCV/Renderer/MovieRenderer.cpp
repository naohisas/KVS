#include "MovieRenderer.h"
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>


namespace kvs
{

namespace opencv
{

MovieRenderer::MovieRenderer():
    m_enable_auto_play( false ),
    m_enable_loop_play( false ),
    m_enable_reverse_play( false ),
    m_frame_index( 0 ),
    m_current_frame_index( 0 )
{
    BaseClass::setEnabledCentering( true );
    BaseClass::setEnabledMirroring( false );
}

void MovieRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::opencv::VideoObject* video = kvs::opencv::VideoObject::DownCast( object );
    const IplImage* frame = video->device().queryFrame();
    if ( !frame ) { return; }

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.enable( GL_TEXTURE_2D );

    if ( !BaseClass::texture().isValid() ) { BaseClass::createTexture( video ); }
    if ( BaseClass::isEnabledCentering() ) { BaseClass::alignCenter( camera ); }

    const int width = frame->width;
    const int height = frame->height;
    const char* data = frame->imageData; // BGRBGRBGR...
    BaseClass::texture().bind();
    BaseClass::texture().load( width, height, data );
    BaseClass::textureMapping();
    BaseClass::texture().unbind();

    this->updateFrameIndex( video );

    kvs::OpenGL::SetClearDepth( 1000 );
    BaseClass::stopTimer();
}

void MovieRenderer::updateFrameIndex( kvs::opencv::VideoObject* video )
{
    if ( m_enable_auto_play )
    {
        m_current_frame_index = m_frame_index;
        if ( m_enable_reverse_play )
        {
            this->decrement_frame_index( video );
        }
        else
        {
            this->increment_frame_index( video );
        }
    }
    else
    {
        const int nframes = static_cast<int>( video->device().numberOfFrames() );
        const int index = kvs::Math::Clamp( m_frame_index, 0, nframes - 1 );
        video->device().setNextFrameIndex( index );
    }
}

void MovieRenderer::increment_frame_index( kvs::opencv::VideoObject* video )
{
    m_frame_index++;

    if ( m_enable_loop_play )
    {
        if ( video->device().nextFrameIndex() == video->device().numberOfFrames() )
        {
            video->device().setNextFrameIndex(0);
            m_frame_index = 0;
        }
    }
    else
    {
        if ( video->device().nextFrameIndex() == video->device().numberOfFrames() )
        {
            video->device().setNextFrameIndex( video->device().numberOfFrames() - 1 );
            m_frame_index = video->device().numberOfFrames() - 1;
        }
    }
}

void MovieRenderer::decrement_frame_index( kvs::opencv::VideoObject* video )
{
    m_frame_index--;

    int reverse_play_index = video->device().nextFrameIndex() - 2;
    if ( m_enable_loop_play )
    {
        if ( video->device().nextFrameIndex() == 1 )
        {
            reverse_play_index = video->device().numberOfFrames() - 1;
            m_frame_index = video->device().numberOfFrames() - 1;
        }
    }
    else
    {
        if ( video->device().nextFrameIndex() == 1 )
        {
            reverse_play_index = 0;
            m_frame_index = 0;
        }
    }
    video->device().setNextFrameIndex( reverse_play_index );
}

} // end of namespace opencv

} // end of namespace kvs
