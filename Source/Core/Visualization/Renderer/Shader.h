/****************************************************************************/
/**
 *  @file   Shader.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Shader.h 1802 2014-08-07 09:22:11Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SHADER_H_INCLUDE
#define KVS__SHADER_H_INCLUDE

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
        LambertShading,     ///< Lambertian shading
        PhongShading,       ///< Phong shading
        BlinnPhongShading   ///< Blinn-Phong shading
    };

    struct Base
    {
        kvs::Vector3f camera_position; ///< camera position in the object coordinate
        kvs::Vector3f light_position; ///< light position in the object coordinate
        float Ka; ///< ambient coefficient
        float Kd; ///< diffuse coefficient
        float Ks; ///< specular coefficient
        float S;  ///< shininess

        Base();
        virtual ~Base();

        virtual void set( const kvs::Camera* camera, const kvs::Light* light, const kvs::ObjectBase* object ) = 0;
        virtual Shader::Type type() const = 0;
        virtual const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vector3f& vertex,
            const kvs::Vector3f& normal ) const = 0;
        virtual float attenuation( const kvs::Vector3f& vertex, const kvs::Vector3f& gradient ) const = 0;
    };

public:

    /*KVS_DEPRECATED*/ typedef Base shader_type;
    typedef Base ShadingModel;

public:

    struct Lambert : public Base
    {
        Lambert();
        Lambert( const Lambert& shader );
        Lambert( const float ka, const float kd );

        void set( const kvs::Camera* camera, const kvs::Light* light, const kvs::ObjectBase* object );
        Shader::Type type() const;
        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vector3f& vertex,
            const kvs::Vector3f& normal ) const;
        float attenuation( const kvs::Vector3f& vertex, const kvs::Vector3f& gradient ) const;
    };

    struct Phong : public Base
    {
        Phong();
        Phong( const Phong& shader );
        Phong( const float ka, const float kd, const float ks, const float s );

        void set( const kvs::Camera* camera, const kvs::Light* light, const kvs::ObjectBase* object );
        Shader::Type type() const;
        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vector3f& vertex,
            const kvs::Vector3f& normal ) const;
        float attenuation( const kvs::Vector3f& vertex, const kvs::Vector3f& gradient ) const;
    };

    struct BlinnPhong : public Base
    {
        BlinnPhong();
        BlinnPhong( const BlinnPhong& shader );
        BlinnPhong( const float ka, const float kd, const float ks, const float s );

        void set( const kvs::Camera* camera, const kvs::Light* light, const kvs::ObjectBase* object );
        Shader::Type type() const;
        const kvs::RGBColor shadedColor(
            const kvs::RGBColor& color,
            const kvs::Vector3f& vertex,
            const kvs::Vector3f& normal ) const;
        float attenuation( const kvs::Vector3f& vertex, const kvs::Vector3f& gradient ) const;
    };
};

} // end of namespace kvs

#endif // KVS__SHADER_H_INCLUDE
