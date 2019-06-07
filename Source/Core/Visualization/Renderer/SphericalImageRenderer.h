#pragma once
#include <kvs/Module>
#include <kvs/ImageRenderer>
#include <kvs/ProgramObject>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

class SphericalImageRenderer : public kvs::ImageRenderer
{
    kvsModule( kvs::SphericalImageRenderer, Renderer );
    kvsModuleBaseClass( kvs::ImageRenderer );

private:
    kvs::ProgramObject m_shader_program; ///< shader program

public:
    SphericalImageRenderer() {}
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void create_shader_program();
};

} // end of namespace kvs
