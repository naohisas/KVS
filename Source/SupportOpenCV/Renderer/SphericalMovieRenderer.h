#pragma once
#include <kvs/Module>
#include <kvs/opencv/MovieRenderer>
#include <kvs/ProgramObject>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

namespace opencv
{

class SphericalMovieRenderer : public kvs::opencv::MovieRenderer
{
    kvsModule( kvs::opencv::SphericalMovieRenderer, Renderer );
    kvsModuleBaseClass( kvs::opencv::MovieRenderer );

private:
    kvs::ProgramObject m_shader_program; ///< shader program

public:
    SphericalMovieRenderer() {}
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void create_shader_program();
};

} // end of namespace opencv

} // end of namespace kvs
