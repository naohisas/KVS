/*****************************************************************************/
/**
 *  @file   StreamlineBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StreamlineBase.h 1778 2014-05-28 10:16:57Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__STREAMLINE_BASE_H_INCLUDE
#define KVS__STREAMLINE_BASE_H_INCLUDE

#include <kvs/Module>
#include <kvs/MapperBase>
#include <kvs/LineObject>
#include <kvs/PointObject>
#include <kvs/StructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  Streamline class.
 */
/*===========================================================================*/
class StreamlineBase : public kvs::MapperBase, public kvs::LineObject
{
    kvsModule( kvs::StreamlineBase, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::LineObject );

public:

    enum IntegrationMethod
    {
        Euler = 0,
        RungeKutta2nd = 1,
        RungeKutta4th = 2
    };

    enum IntegrationDirection
    {
        BackwardDirection = -1,
        BothDirections = 0,
        ForwardDirection = 1
    };

protected:

    kvs::PointObject* m_seed_points; ///< seed points
    IntegrationMethod m_integration_method; ///< integtration method
    IntegrationDirection m_integration_direction; ///< integration direction
    float m_integration_interval; ///< integration interval in the object coordinate
    float m_vector_length_threshold; ///< threshold of the vector length
    size_t m_integration_times_threshold; ///< threshold of the integration times
    bool m_enable_boundary_condition; ///< flag for the boundray condition
    bool m_enable_vector_length_condition; ///< flag for the vector length condition
    bool m_enable_integration_times_condition; ///< flag for the integration times

public:

    StreamlineBase();
    virtual ~StreamlineBase();

    void setSeedPoints( const kvs::PointObject* seed_points );
    void setIntegrationMethod( const IntegrationMethod method ) { m_integration_method = method; }
    void setIntegrationDirection( const IntegrationDirection direction ) { m_integration_direction = direction; }
    void setIntegrationInterval( const float interval ) { m_integration_interval = interval; }
    void setVectorLengthThreshold( const float length ) { m_vector_length_threshold = length; }
    void setIntegrationTimesThreshold( const size_t times ) { m_integration_times_threshold = times; }
    void setEnableBoundaryCondition( const bool enabled ) { m_enable_boundary_condition = enabled; }
    void setEnableVectorLengthCondition( const bool enabled ) { m_enable_vector_length_condition = enabled; }
    void setEnableIntegrationTimesCondition( const bool enabled ) { m_enable_integration_times_condition = enabled; }

    virtual kvs::ObjectBase* exec( const kvs::ObjectBase* object ) = 0;

protected:

    virtual bool check_for_acceptance( const std::vector<kvs::Real32>& vertices ) = 0;
    virtual bool check_for_termination(
        const kvs::Vec3& current_vertex,
        const kvs::Vec3& direction,
        const size_t integration_times,
        const kvs::Vec3& next_vertex ) = 0;
    virtual const kvs::Vec3 interpolate_vector( const kvs::Vec3& vertex, const kvs::Vec3& direction ) = 0;
    virtual const kvs::Vec3 calculate_vector( const kvs::Vec3& vertex ) = 0;
    virtual const kvs::RGBColor calculate_color( const kvs::Vec3& direction ) = 0;

    void mapping( const kvs::VolumeObjectBase* volume );

    void extract_lines( const kvs::StructuredVolumeObject* volume );
    bool calculate_line( std::vector<kvs::Real32>* vertices, std::vector<kvs::UInt8>* colors, const size_t index );
    bool calculate_one_side(
        std::vector<kvs::Real32>* coords,
        std::vector<kvs::UInt8>* colors,
        const kvs::Vec3& seed_point,
        const kvs::Vec3& seed_vector );
    bool calculate_next_vertex(
        const kvs::Vec3& current_vertex,
        const kvs::Vec3& current_direction,
        kvs::Vec3* next_vertex );
    bool integrate_by_euler(
        const kvs::Vec3& current_vertex,
        const kvs::Vec3& current_direction,
        kvs::Vec3* next_vertex );
    bool integrate_by_runge_kutta_2nd(
        const kvs::Vec3& current_vertex,
        const kvs::Vec3& current_direction,
        kvs::Vec3* next_vertex );
    bool integrate_by_runge_kutta_4th(
        const kvs::Vec3& current_vertex,
        const kvs::Vec3& current_direction,
        kvs::Vec3* next_vertex );

    bool check_for_inside_volume( const kvs::Vec3& seed );
    bool check_for_vector_length( const kvs::Vec3& direction );
    bool check_for_integration_times( const size_t times );
};

} // end of namespace kvs

#endif // KVS__STREAMLINE_BASE_H_INCLUDE
