/*****************************************************************************/
/**
 *  @file   StreamlineBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StreamlineBase.cpp 1779 2014-05-30 08:23:40Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "StreamlineBase.h"
#include <kvs/DebugNew>
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new streamline class.
 */
/*===========================================================================*/
StreamlineBase::StreamlineBase():
    kvs::MapperBase(),
    m_seed_points( NULL ),
    m_integration_method( StreamlineBase::RungeKutta2nd ),
    m_integration_direction( StreamlineBase::ForwardDirection ),
    m_integration_interval( 1.0f ),
    m_vector_length_threshold( 0.000001f ),
    m_integration_times_threshold( 1000 ),
    m_enable_boundary_condition( true ),
    m_enable_vector_length_condition( true ),
    m_enable_integration_times_condition( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the streamline class.
 */
/*===========================================================================*/
StreamlineBase::~StreamlineBase()
{
    delete m_seed_points;
}

/*===========================================================================*/
/**
 *  @brief  Set the seed points.
 *  @param  seed_points [in] pointer to the seed points
 */
/*===========================================================================*/
void StreamlineBase::setSeedPoints( const kvs::PointObject* seed_points )
{
    m_seed_points = new kvs::PointObject();
    m_seed_points->setCoords( seed_points->coords() ); // shallow copy
}

void StreamlineBase::mapping( Integrator* integrator )
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt8> colors;
    std::vector<kvs::UInt32> connections;

    for ( size_t i = 0; i < m_seed_points->numberOfVertices(); i++ )
    {
        kvs::Vec3 point = m_seed_points->coord( i );
        if ( !integrator->contains( point ) ) { continue; }

        kvs::Vec3 value = integrator->value( point );
        if ( this->isTerminatedByVectorLength( value ) ) { continue; }

        kvs::RGBColor color = this->interpolatedColor( value );
        coords.push_back( point.x() );
        coords.push_back( point.y() );
        coords.push_back( point.z() );
        colors.push_back( color.r() );
        colors.push_back( color.g() );
        colors.push_back( color.b() );

        const size_t id0 = coords.size() / 3 - 1;
        for ( size_t j = 0; !this->isTerminatedByIntegrationTimes(j); j++ )
        {
            point = integrator->next( point );
            if ( !integrator->contains( point ) ) { break; }

            value = integrator->value( point );
            if ( this->isTerminatedByVectorLength( value ) ) { break; }

            color = this->interpolatedColor( value );
            coords.push_back( point.x() );
            coords.push_back( point.y() );
            coords.push_back( point.z() );
            colors.push_back( color.r() );
            colors.push_back( color.g() );
            colors.push_back( color.b() );
        }
        const size_t id1 = coords.size() / 3 - 1;

        if ( id0 != id1 )
        {
            connections.push_back( id0 );
            connections.push_back( id1 );
        }
    }

    SuperClass::setLineType( kvs::LineObject::Polyline );
    SuperClass::setColorType( kvs::LineObject::VertexColor );
    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setConnections( kvs::ValueArray<kvs::UInt32>( connections ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setSize( 1.0f );
}

kvs::RGBColor StreamlineBase::interpolatedColor( const kvs::Vec3& value )
{
    return BaseClass::transferFunction().colorMap().at( value.length() );
}

bool StreamlineBase::isTerminatedByVectorLength( const kvs::Vec3& vector )
{
    if ( m_enable_vector_length_condition )
    {
        return vector.length() < m_vector_length_threshold;
    }

    return false;
}

bool StreamlineBase::isTerminatedByIntegrationTimes( const size_t times )
{
    if ( m_enable_integration_times_condition )
    {
        return times >= m_integration_times_threshold;
    }

    return false;
}

} // end of namespace kvs
