/*****************************************************************************/
/**
 *  @file   Streamline.cpp
 *  @author Yukio Yasuhra, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Streamline.cpp 1778 2014-05-28 10:16:57Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Streamline.h"
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Message>
#include <kvs/RGBColor>
#include <kvs/Vector3>
#include <kvs/VolumeObjectBase>


namespace
{

template <typename T>
inline const kvs::Vec3 GetInterpolatedVector(
    const size_t vertex_id[8],
    const float weight[8],
    const kvs::VolumeObjectBase* volume )
{
    const T* values = reinterpret_cast<const T*>( volume->values().data() );

    kvs::Vec3 ret( 0.0f, 0.0f, 0.0f );
    for ( size_t i = 0; i < 8; i++ )
    {
        const size_t index = 3 * vertex_id[i];
        const float w = weight[i];
        ret.x() += static_cast<float>( values[ index     ] * w );
        ret.y() += static_cast<float>( values[ index + 1 ] * w );
        ret.z() += static_cast<float>( values[ index + 2 ] * w );
    }

    return ret;
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new streamline class.
 */
/*===========================================================================*/
Streamline::Streamline():
    kvs::StreamlineBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new streamline class and executes this class.
 *  @param  volume [in] pointer to the input volume object
 *  @param  seed_points [in] pointer to the seed points
 */
/*===========================================================================*/
Streamline::Streamline(
    const kvs::StructuredVolumeObject* volume,
    const kvs::PointObject* seed_points,
    const kvs::TransferFunction& transfer_function ):
    kvs::StreamlineBase()
{
    BaseClass::setTransferFunction( transfer_function );
    BaseClass::setSeedPoints( seed_points );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the streamline class.
 */
/*===========================================================================*/
Streamline::~Streamline()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return line object
 */
/*===========================================================================*/
Streamline::BaseClass::SuperClass* Streamline::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 3 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not vector field data.");
        return NULL;
    }

    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // set the min/max vector length.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    BaseClass::mapping( volume );

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Check whether the vertices are accepted or not.
 *  @param  vertices [in] vertices
 *  @return true if the vertices are accepted
 */
/*===========================================================================*/
bool Streamline::check_for_acceptance( const std::vector<kvs::Real32>& vertices )
{
    kvs::IgnoreUnusedVariable( vertices );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Test whether the streamline is futher integrated or terminated.
 *  @param  current_vertex [in] position of the current vertex
 *  @param  direction [in] direction vector
 *  @param  integration_times [in] integration times
 *  @param  next_vertex [in] position of the next vertex
 *  @return true, if the stremaline is integrated.
 */
/*===========================================================================*/
bool Streamline::check_for_termination(
    const kvs::Vec3& current_vertex,
    const kvs::Vec3& direction,
    const size_t integration_times,
    const kvs::Vec3& next_vertex )
{
    kvs::IgnoreUnusedVariable( current_vertex );

    if ( m_enable_boundary_condition )
    {
        if ( !BaseClass::check_for_inside_volume( next_vertex ) ) return true;
    }

    if ( m_enable_vector_length_condition )
    {
//        return BaseClass::check_for_vector_length( direction );
        if ( BaseClass::check_for_vector_length( direction ) ) return true;
    }

    if ( m_enable_integration_times_condition )
    {
//        return BaseClass::check_for_integration_times( integration_times );
        if ( BaseClass::check_for_integration_times( integration_times ) ) return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Calculate the interpolated vector on the seed point.
 *  @param  point [in] seed point
 *  @return interpolated vector
 */
/*===========================================================================*/
const kvs::Vec3 Streamline::calculate_vector( const kvs::Vec3& point )
{
    const kvs::Vec3 origin( 0.0f, 0.0f, 0.0f );
    return this->interpolate_vector( point, origin );
}

/*===========================================================================*/
/**
 *  @brief  Calculate the color.
 *  @param  direction [in] direction vector
 *  @return color
 */
/*===========================================================================*/
const kvs::RGBColor Streamline::calculate_color( const kvs::Vec3& direction )
{
    const kvs::Real64 min_length = BaseClass::volume()->minValue();
    const kvs::Real64 max_length = BaseClass::volume()->maxValue();
    const kvs::Real64 diff = direction.length() - min_length;
    const kvs::Real64 interval = max_length - min_length;
    const kvs::UInt8 level = kvs::UInt8( 255.0 * diff / interval );

    return BaseClass::transferFunction().colorMap()[level];
}

/*===========================================================================*/
/**
 *  @brief  Interpolates the vertex.
 *  @param  vertex [in] vertex
 *  @param  previous_vector [in] previous vector
 *  @return interpolated vector
 */
/*===========================================================================*/
const kvs::Vec3 Streamline::interpolate_vector(
    const kvs::Vec3& vertex,
    const kvs::Vec3& previous_vector )
{
    kvs::IgnoreUnusedVariable( previous_vector );

    const size_t cell_x = static_cast<size_t>( vertex.x() );
    const size_t cell_y = static_cast<size_t>( vertex.y() );
    const size_t cell_z = static_cast<size_t>( vertex.z() );

    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( BaseClass::volume() );
    const size_t resolution_x = static_cast<size_t>( volume->resolution().x() );
    const size_t resolution_y = static_cast<size_t>( volume->resolution().y() );
//    const size_t resolution_z = static_cast<size_t>( volume->resolution().z() );

    size_t vertex_id[8];
    vertex_id[0] = cell_z * resolution_x * resolution_y + cell_y * resolution_x + cell_x;
    vertex_id[1] = vertex_id[0] + 1;
    vertex_id[2] = vertex_id[1] + resolution_x;
    vertex_id[3] = vertex_id[2] - 1;
    vertex_id[4] = vertex_id[0] + resolution_x * resolution_y;
    vertex_id[5] = vertex_id[4] + 1;
    vertex_id[6] = vertex_id[5] + resolution_x;
    vertex_id[7] = vertex_id[6] - 1;

    // Weight.
    const kvs::Vec3 local_coord(
        2.0f * ( vertex.x() - cell_x ) - 1.0f,
        2.0f * ( vertex.y() - cell_y ) - 1.0f,
        2.0f * ( vertex.z() - cell_z ) - 1.0f );

    const float x_min = local_coord.x() - 1.0f;
    const float x_max = local_coord.x() + 1.0f;
    const float y_min = local_coord.y() - 1.0f;
    const float y_max = local_coord.y() + 1.0f;
    const float z_min = local_coord.z() - 1.0f;
    const float z_max = local_coord.z() + 1.0f;

    const float weight[8] = {
        -x_min * y_min * z_min * 0.125f,
        x_max  * y_min * z_min * 0.125f,
        -x_max * y_max * z_min * 0.125f,
        x_min  * y_max * z_min * 0.125f,
        x_min  * y_min * z_max * 0.125f,
        -x_max * y_min * z_max * 0.125f,
        x_max  * y_max * z_max * 0.125f,
        -x_min * y_max * z_max * 0.125f };

    // Interpolate.
    const std::type_info& type = BaseClass::volume()->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) return ::GetInterpolatedVector<kvs::Int8>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::Int16  ) ) return ::GetInterpolatedVector<kvs::Int16>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::Int32  ) ) return ::GetInterpolatedVector<kvs::Int32>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::Int64  ) ) return ::GetInterpolatedVector<kvs::Int64>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::UInt8  ) ) return ::GetInterpolatedVector<kvs::UInt8>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::UInt16 ) ) return ::GetInterpolatedVector<kvs::UInt16>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::UInt32 ) ) return ::GetInterpolatedVector<kvs::UInt32>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::UInt64 ) ) return ::GetInterpolatedVector<kvs::UInt64>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::Real32 ) ) return ::GetInterpolatedVector<kvs::Real32>( vertex_id, weight, BaseClass::volume() );
    else if ( type == typeid( kvs::Real64 ) ) return ::GetInterpolatedVector<kvs::Real64>( vertex_id, weight, BaseClass::volume() );

    return kvs::Vec3( 0.0f, 0.0f, 0.0f );
}

} // end of namespace kvs
