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
//        BothDirections = 0,
        ForwardDirection = 1
    };

    class Interpolator
    {
    public:
        virtual ~Interpolator() {}
        virtual kvs::Vec3 interpolatedValue( const kvs::Vec3& point ) = 0;
        virtual bool containsInVolume( const kvs::Vec3& point ) = 0;
        kvs::Vec3 direction( const kvs::Vec3& point )
        {
//            return this->interpolatedValue( point ).normalized();
            return this->interpolatedValue( point );
        }
    };

    class Integrator
    {
    private:
        float m_step;
        Interpolator* m_interpolator;
    public:
        virtual ~Integrator() {}
        virtual kvs::Vec3 next( const kvs::Vec3& point ) = 0;
        void setStep( const float step ) { m_step = step; }
        void setInterpolator( Interpolator* interpolator ) { m_interpolator = interpolator; }
        float step() const { return m_step; }
        bool contains( const kvs::Vec3& point ) { return m_interpolator->containsInVolume( point ); }
        kvs::Vec3 value( const kvs::Vec3& point ) { return m_interpolator->interpolatedValue( point ); }
//        kvs::Vec3 direction( const kvs::Vec3& point ) { return this->value( point ).normalized(); }
        kvs::Vec3 direction( const kvs::Vec3& point ) { return this->value( point ); }
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

    IntegrationMethod integrationMethod() const { return m_integration_method; }
    IntegrationDirection integrationDirection() const { return m_integration_direction; }
    float integrationInterval() const { return m_integration_interval; }

    virtual kvs::ObjectBase* exec( const kvs::ObjectBase* object ) = 0;

protected:

    void mapping( Integrator* integrator );
    kvs::RGBColor interpolatedColor( const kvs::Vec3& value );
    bool isTerminatedByVectorLength( const kvs::Vec3& vector );
    bool isTerminatedByIntegrationTimes( const size_t times );
};

} // end of namespace kvs

#endif // KVS__STREAMLINE_BASE_H_INCLUDE
