/****************************************************************************/
/**
 *  @file   OpacityMap.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpacityMap.cpp 1619 2013-07-18 05:12:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "OpacityMap.h"
#include <kvs/Assert>
#include <kvs/Math>


namespace
{

const size_t Resolution = 256;

struct Equal
{
    float value;

    Equal( const float v ) : value( v ){}

    bool operator() ( const kvs::OpacityMap::Point& point ) const
    {
        return kvs::Math::Equal( point.first, value );
    }
};

struct Less
{
    bool operator() ( const kvs::OpacityMap::Point& p1, const kvs::OpacityMap::Point& p2 ) const
    {
        return p1.first < p2.first;
    }
};

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 */
/*==========================================================================*/
OpacityMap::OpacityMap():
    m_resolution( ::Resolution ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 *  @param  resolution [in] table resolution
 */
/*===========================================================================*/
OpacityMap::OpacityMap( const size_t resolution ):
    m_resolution( resolution ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 *  @param  table [in] opacity table
 */
/*===========================================================================*/
OpacityMap::OpacityMap( const Table& table ):
    m_resolution( table.size() ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table( table )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 *  @param  resolution [in] resolution
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
OpacityMap::OpacityMap( const size_t resolution, const float min_value, const float max_value ):
    m_resolution( resolution ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 *  @param  table [in] opacity value table
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
OpacityMap::OpacityMap( const OpacityMap::Table& table, const float min_value, const float max_value ):
    m_resolution( table.size() ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table( table )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new OpacityMap class.
 *  @param  opacity_map [in] opacity map
 */
/*==========================================================================*/
OpacityMap::OpacityMap( const OpacityMap& other ):
    m_resolution( other.m_resolution ),
    m_min_value( other.m_min_value ),
    m_max_value( other.m_max_value ),
    m_points( other.m_points ),
    m_table( other.m_table )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the range is specified.
 *  @return ture if the range is specified
 */
/*===========================================================================*/
bool OpacityMap::hasRange() const
{
    return !kvs::Math::Equal( m_min_value, m_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Adds a control point.
 *  @param  value [in] scalar value in [min_value, max_value]
 *  @param  opacity [in] opacity value in [0,1]
 */
/*===========================================================================*/
void OpacityMap::addPoint( const float value, const float opacity )
{
    m_points.push_back( Point( value, kvs::Math::Clamp( opacity, 0.0f, 1.0f ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Removes the constrol point associated with the given scalar value.
 *  @param  value [in] scalar value in [min_value, max_value]
 */
/*===========================================================================*/
void OpacityMap::removePoint( const float value )
{
    m_points.remove_if( ::Equal( value ) );
}

/*==========================================================================*/
/**
 *  @brief  Creates the opacity map.
 */
/*==========================================================================*/
void OpacityMap::create()
{
    kvs::Real32 min_value = 0.0f;
    kvs::Real32 max_value = static_cast<kvs::Real32>( m_resolution - 1 );
    if ( this->hasRange() )
    {
        min_value = this->minValue();
        max_value = this->maxValue();
    }

    m_table.allocate( m_resolution );
    if ( m_points.size() == 0 )
    {
        const float scale = 1.0f / static_cast<float>( m_resolution - 1 );
        for ( size_t i = 0; i < m_resolution; ++i )
        {
            m_table[i] = scale * static_cast<float>( i );
        }
    }
    else
    {
        m_points.sort( ::Less() );

        if ( m_points.front().first > min_value ) this->addPoint( min_value, 0.0f );
        if ( m_points.back().first < max_value ) this->addPoint( max_value, 1.0f );

        const float stride = ( max_value - min_value ) / static_cast<float>( m_resolution - 1 );
        float f = min_value;
        for ( size_t i = 0; i < m_resolution; ++i, f += stride )
        {
            Points::iterator p = m_points.begin();
            Points::iterator last = m_points.end();

            float opacity = 0.0f;
            Point p0( min_value, 0.0f );
            Point p1( max_value, 1.0f );
            while ( p != last )
            {
                const float s = p->first;
                if ( kvs::Math::Equal( f, s ) )
                {
                    opacity = p->second;
                    break;
                }
                else if ( f < s )
                {
                    p1 = *p;
                    // Interpolate.
                    const float s0 = p0.first;
                    const float s1 = p1.first;
                    const float a0 = p0.second;
                    const float a1 = p1.second;
                    opacity = kvs::Math::Mix( a0, a1, ( f - s0 ) / ( s1 - s0 ) );
                    break;
                }
                else
                {
                    p0 = *p;
                    ++p;
                    if ( p == last )
                    {
                        if ( kvs::Math::Equal( p0.first, max_value ) )
                        {
                            opacity = p0.second;
                        }
                    }
                }
            }

            m_table[i] = opacity;
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns the opacity value which is specified by the table index.
 *  @param  index [in] table index
 *  @return opacity value
 */
/*==========================================================================*/
kvs::Real32 OpacityMap::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->resolution() );
    return m_table[index];
}

/*===========================================================================*/
/**
 *  @brief  Returns interpolated opacity value by assuming piecewise linear map.
 *  @param  value [in] value
 *  @return interpolated opacity value
 */
/*===========================================================================*/
kvs::Real32 OpacityMap::at( const float value ) const
{
    if ( value < m_min_value || m_max_value < value ) { return 0.0f; }

    const float r = static_cast<float>( m_resolution - 1 );
    const float v = ( value - m_min_value ) / ( m_max_value - m_min_value ) * r;
    const size_t s0 = static_cast<size_t>( v );
    const size_t s1 = kvs::Math::Min( s0 + 1, m_resolution - 1 );

    const kvs::Real32 a0 = m_table[ s0 ];
    const kvs::Real32 a1 = m_table[ s1 ];
    return kvs::Math::Mix( a0, a1, v - s0 );
}

/*==========================================================================*/
/**
 *  Substitution operator =.
 *  @param opacity_map [in] opacity map
 *  @retval opacity map
 */
/*==========================================================================*/
OpacityMap& OpacityMap::operator =( const OpacityMap& rhs )
{
    m_resolution = rhs.m_resolution;
    m_min_value = rhs.m_min_value;
    m_max_value = rhs.m_max_value;
    m_points = rhs.m_points;
    m_table = rhs.m_table;
    return *this;
}

} // end of namespace kvs
