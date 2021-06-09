/****************************************************************************/
/**
 *  @file   Shader.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Shader.h"
#include <kvs/Math>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/RGBColor>
#include <kvs/Coordinate>


#define kvsShaderAmbientTerm( ka ) \
    ka

#define kvsShaderDiffuseTerm( kd, N, L ) \
    kd * kvs::Math::Max( N.dot( L ), 0.0f )

#define kvsShaderSpecularTerm( ks, S, R, V ) \
    ks * std::pow( kvs::Math::Max( R.dot( V ), 0.0f ), S )


namespace
{

inline const kvs::RGBColor Shade(
    const kvs::RGBColor& C,
    const float Ia,
    const float Id,
    const float Is )
{
    const auto I1 = Ia + Id;
    const auto I2 = Is * 255.0f;
    const auto r = static_cast<kvs::UInt8>( kvs::Math::Min( C.r() * I1 + I2, 255.0f ) + 0.5f );
    const auto g = static_cast<kvs::UInt8>( kvs::Math::Min( C.g() * I1 + I2, 255.0f ) + 0.5f );
    const auto b = static_cast<kvs::UInt8>( kvs::Math::Min( C.b() * I1 + I2, 255.0f ) + 0.5f );
    return kvs::RGBColor( r, g, b );
}

} // end of namespace

namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Set the camera and light.
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 *  @param  object [in] pointer to the object
 */
/*==========================================================================*/
void Shader::Lambert::set(
    const kvs::Camera* camera,
    const kvs::Light* light,
    const kvs::ObjectBase* object )
{
    kvs::IgnoreUnusedVariable( camera );
    light_position = kvs::WorldCoordinate( light->position() ).toObjectCoordinate( object ).position();
}

/*===========================================================================*/
/**
 *  @brief  Returns shaded color.
 *  @param  color [in] source color
 *  @param  vertex [in] vertex position
 *  @param  normal [in] normal vector
 *  @return shaded color
 */
/*===========================================================================*/
const kvs::RGBColor Shader::Lambert::shadedColor(
    const kvs::RGBColor& color,
    const kvs::Vec3& vertex,
    const kvs::Vec3& normal ) const
{
    // Light vector L and normal vector N.
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 N = normal.normalized();

    // Intensity values.
    const float Ia = kvsShaderAmbientTerm( Base::Ka );
    const float Id = kvsShaderDiffuseTerm( Base::Kd, N, L );

    return color * ( Ia + Id );
}

/*==========================================================================*/
/**
 *  Get the attenuation value.
 *  @return attenuation value
 */
/*==========================================================================*/
inline float Shader::Lambert::attenuation(
    const kvs::Vec3& vertex,
    const kvs::Vec3& gradient ) const
{
    // Light vector L and normal vector N.
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 N = gradient.normalized();

    const float dd = kvs::Math::Max( N.dot( L ), 0.0f );

    /* I = Ia + Id
     *
     * Ia = Ka (constant term)
     * Id = Ip *  Kd * cos(A) = Ip * Kd * ( L dot N )
     *
     * Ip : the intensity emitted from the light source.
     */
    const float Ia = Base::Ka;
    const float Id = Base::Kd * dd;

    return Ia + Id;
}

/*==========================================================================*/
/**
 *  Set the camera and light.
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 *  @param object [in] pointer to the object
 */
/*==========================================================================*/
void Shader::Phong::set(
    const kvs::Camera* camera,
    const kvs::Light* light,
    const kvs::ObjectBase* object )
{
    camera_position = kvs::WorldCoordinate( camera->position() ).toObjectCoordinate( object ).position();
    light_position = kvs::WorldCoordinate( light->position() ).toObjectCoordinate( object ).position();
}

/*===========================================================================*/
/**
 *  @brief  Returns shaded color.
 *  @param  color [in] source color
 *  @param  vertex [in] vertex position
 *  @param  normal [in] normal vector
 *  @return shaded color
 */
/*===========================================================================*/
const kvs::RGBColor Shader::Phong::shadedColor(
    const kvs::RGBColor& color,
    const kvs::Vec3& vertex,
    const kvs::Vec3& normal ) const
{
    // Light vector L, normal vector N and reflection vector R.
    const kvs::Vec3 V = ( camera_position - vertex ).normalized();
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 N = normal.normalized();
    const kvs::Vec3 R = 2.0f * N.dot( L ) * N - L;

    // Intensity values.
    const float Ia = kvsShaderAmbientTerm( Base::Ka );
    const float Id = kvsShaderDiffuseTerm( Base::Kd, N, L );
    const float Is = kvsShaderSpecularTerm( Base::Ks, Base::S, R, V );

    return ::Shade( color, Ia, Id, Is );
}

/*==========================================================================*/
/**
 *  Get the attenuation value.
 *  @return attenuation value
 */
/*==========================================================================*/
inline float Shader::Phong::attenuation(
    const kvs::Vec3& vertex,
    const kvs::Vec3& gradient ) const
{
    // Light vector L, normal vector N and reflection vector R.
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 N = gradient.normalized();
    const kvs::Vec3 R = 2.0f * N.dot( L ) * N - L;

    const float dd = kvs::Math::Max( N.dot( L ), 0.0f );
    const float ds = kvs::Math::Max( N.dot( R ), 0.0f );

    /* I = Ia + Id + Is
     *
     * Is = Ip * Ks * cos^s(B) = Ip * Ks * ( R dot N )^s
     */
    const float Ia = Base::Ka;
    const float Id = Base::Kd * dd;
    const float Is = Base::Ks * std::pow( ds, Base::S );

    return Ia + Id + Is;
}

/*==========================================================================*/
/**
 *  Set the camera and light
 *  @param camera [in] pointer to the camera
 *  @param light [in] pointer to the light
 *  @param object [in] pointer to the object
 */
/*==========================================================================*/
void Shader::BlinnPhong::set(
    const kvs::Camera* camera,
    const kvs::Light* light,
    const kvs::ObjectBase* object )
{
    camera_position = kvs::WorldCoordinate( camera->position() ).toObjectCoordinate( object ).position();
    light_position = kvs::WorldCoordinate( light->position() ).toObjectCoordinate( object ).position();
}

/*===========================================================================*/
/**
 *  @brief  Returns shaded color.
 *  @param  color [in] source color
 *  @param  vertex [in] vertex position
 *  @param  normal [in] normal vector
 *  @return shaded color
 */
/*===========================================================================*/
const kvs::RGBColor Shader::BlinnPhong::shadedColor(
    const kvs::RGBColor& color,
    const kvs::Vec3& vertex,
    const kvs::Vec3& normal ) const
{
    // Camera vector V, light vector L, halfway vector H and normal vector N.
    const kvs::Vec3 V = ( camera_position - vertex ).normalized();
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 H = ( V + L ).normalized();
    const kvs::Vec3 N = normal.normalized();

    // Intensity values.
    const float Ia = kvsShaderAmbientTerm( Base::Ka );
    const float Id = kvsShaderDiffuseTerm( Base::Kd, N, L );
    const float Is = kvsShaderSpecularTerm( Base::Ks, Base::S, H, N );

    return ::Shade( color, Ia, Id, Is );
}

/*==========================================================================*/
/**
 *  Get the attenuation value.
 *  @return attenuation value
 */
/*==========================================================================*/
inline float Shader::BlinnPhong::attenuation(
    const kvs::Vec3& vertex,
    const kvs::Vec3& gradient ) const
{
    // Camera vector C, light vector L, halfway vector H and normal vector N.
    const kvs::Vec3 C = ( camera_position - vertex ).normalized();
    const kvs::Vec3 L = ( light_position - vertex ).normalized();
    const kvs::Vec3 H = ( C + L ).normalized();
    const kvs::Vec3 N = gradient.normalized();

    const float dd = kvs::Math::Max( N.dot( L ), 0.0f );
    const float ds = kvs::Math::Max( N.dot( H ), 0.0f );

    /* I = Ia + Id + Is
     *
     * Is = Ip * Ks * cos^s(B) = Ip * Ks * ( H dot N )^s
     */
    const float Ia = Base::Ka;
    const float Id = Base::Kd * dd;
    const float Is = Base::Ks * ::pow( ds, Base::S );

    return Ia + Id + Is;
}

} // end of namespace kvs
