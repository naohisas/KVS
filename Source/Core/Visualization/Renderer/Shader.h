/****************************************************************************/
/**
 *  @file   Shader.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/Vector3>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>


namespace kvs
{

/*==========================================================================*/
/**
 *  Shader class.
 */
/*==========================================================================*/
class Shader
{
public:
    enum Type
    {
        UnknownShading = 0, ///< unknown shading type
        LambertShading, ///< Lambertian shading
        PhongShading, ///< Phong shading
        BlinnPhongShading ///< Blinn-Phong shading
    };

    struct Base
    {
        kvs::Vec3 camera_position{}; ///< camera position in the object coordinate
        kvs::Vec3 light_position{}; ///< light position in the object coordinate
        float Ka = 0.0f; ///< ambient coefficient
        float Kd = 0.0f; ///< diffuse coefficient
        float Ks = 0.0f; ///< specular coefficient
        float S = 0.0f; ///< shininess
        bool two_side_lighting = true; ///< flag for two-side lighting

        Base() = default;
        virtual ~Base() = default;

        virtual Shader::Type type() const = 0;

        virtual void set(
            const kvs::Camera* camera,
            const kvs::Light* light,
            const kvs::ObjectBase* object ) = 0;

        virtual const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vec3& vertex,
            const kvs::Vec3& normal ) const = 0;

        virtual float attenuation(
            const kvs::Vec3& vertex,
            const kvs::Vec3& gradient ) const = 0;
    };

public:
    /*KVS_DEPRECATED*/ typedef Base shader_type;
    using ShadingModel = Base;

public:
    struct Lambert : public Base
    {
        Lambert(
            const float ka = 0.4f,
            const float kd = 0.6f )
        {
            Base::Ka = ka;
            Base::Kd = kd;
        }

        Lambert( const Lambert& shader )
        {
            Base::camera_position = shader.camera_position;
            Base::light_position = shader.light_position;
            Base::Ka = shader.Ka;
            Base::Kd = shader.Kd;
        }

        Shader::Type type() const { return Shader::LambertShading; }

        void set(
            const kvs::Camera* camera,
            const kvs::Light* light,
            const kvs::ObjectBase* object );

        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vec3& vertex,
            const kvs::Vec3& normal ) const;

        float attenuation(
            const kvs::Vec3& vertex,
            const kvs::Vec3& gradient ) const;
    };

    struct Phong : public Base
    {
        Phong(
            const float ka = 0.3f,
            const float kd = 0.5f,
            const float ks = 0.8f,
            const float s = 100.0f )
        {
            Base::Ka = ka;
            Base::Kd = kd;
            Base::Ks = ks;
            Base::S = s;
        }

        Phong( const Phong& shader )
        {
            Base::camera_position = shader.camera_position;
            Base::light_position = shader.light_position;
            Base::Ka = shader.Ka;
            Base::Kd = shader.Kd;
            Base::Ks = shader.Ks;
            Base::S = shader.S;
        }

        Shader::Type type() const { return Shader::PhongShading; }

        void set(
            const kvs::Camera* camera,
            const kvs::Light* light,
            const kvs::ObjectBase* object );

        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vec3& vertex,
            const kvs::Vec3& normal ) const;

        float attenuation(
            const kvs::Vec3& vertex,
            const kvs::Vec3& gradient ) const;
    };

    struct BlinnPhong : public Base
    {
        BlinnPhong(
            const float ka = 0.3f,
            const float kd = 0.5f,
            const float ks = 0.8f,
            const float s = 100.0f )
        {
            Base::Ka = ka;
            Base::Kd = kd;
            Base::Ks = ks;
            Base::S = s;
        }

        BlinnPhong( const BlinnPhong& shader )
        {
            Base::camera_position = shader.camera_position;
            Base::light_position = shader.light_position;
            Base::Ka = shader.Ka;
            Base::Kd = shader.Kd;
            Base::Ks = shader.Ks;
            Base::S = shader.S;
        }

        Shader::Type type() const { return Shader::BlinnPhongShading; }

        void set(
            const kvs::Camera* camera,
            const kvs::Light* light,
            const kvs::ObjectBase* object );

        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vec3& vertex,
            const kvs::Vec3& normal ) const;

        float attenuation(
            const kvs::Vec3& vertex,
            const kvs::Vec3& gradient ) const;
    };
};

} // end of namespace kvs
