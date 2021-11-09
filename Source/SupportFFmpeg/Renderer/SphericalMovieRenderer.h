/*****************************************************************************/
/**
 *  @file   SphericalMovieRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "MovieRenderer.h"
#include <kvs/ProgramObject>

namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Spherical movie renderer class.
 */
/*===========================================================================*/
class SphericalMovieRenderer : public kvs::ffmpeg::MovieRenderer
{
    kvsModule( kvs::ffmpeg::SphericalMovieRenderer, Renderer );
    kvsModuleBaseClass( kvs::ffmpeg::MovieRenderer );

private:
    kvs::ProgramObject m_shader_program{}; ///< shader program

public:
    SphericalMovieRenderer() = default;
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void create_shader_program();
};

} // end of namespace ffmpeg

} // end of namespace kvs
