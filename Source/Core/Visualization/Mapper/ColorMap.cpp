/****************************************************************************/
/**
 *  @file   ColorMap.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorMap.cpp 1755 2014-05-02 02:18:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ColorMap.h"
#include <kvs/Assert>
#include <kvs/RGBColor>
#include <kvs/HSVColor>
#include <kvs/Math>


namespace
{

const size_t Resolution = 256;
const size_t NumberOfChannels = 3;

struct Equal
{
    float value;

    Equal( const float v ) : value( v ){}

    bool operator() ( const kvs::ColorMap::Point& point ) const
    {
        return kvs::Math::Equal( point.first, value );
    }
};

struct Less
{
    bool operator() ( const kvs::ColorMap::Point& p1, const kvs::ColorMap::Point& p2 ) const
    {
        return p1.first < p2.first;
    }
};

kvs::RGBColor Interpolate(
    const float s,
    const float s0,
    const float s1,
    const kvs::RGBColor& c0,
    const kvs::RGBColor& c1 )
{
    const float r0 = static_cast<float>( c0.r() );
    const float g0 = static_cast<float>( c0.g() );
    const float b0 = static_cast<float>( c0.b() );

    const float r1 = static_cast<float>( c1.r() );
    const float g1 = static_cast<float>( c1.g() );
    const float b1 = static_cast<float>( c1.b() );

    const float w = ( s - s0 ) / ( s1 - s0 );
    const float r = kvs::Math::Mix( r0, r1, w );
    const float g = kvs::Math::Mix( g0, g1, w );
    const float b = kvs::Math::Mix( b0, b1, w );

    const kvs::UInt8 R = static_cast<kvs::UInt8>( r );
    const kvs::UInt8 G = static_cast<kvs::UInt8>( g );
    const kvs::UInt8 B = static_cast<kvs::UInt8>( b );
    return kvs::RGBColor( R, G, B );
};

kvs::RGBColor Interpolate(
    const float s,
    const float s0,
    const float s1,
    const kvs::HSVColor& c0,
    const kvs::HSVColor& c1 )
{
    const float w = ( s - s0 ) / ( s1 - s0 );
    const float H = kvs::Math::Mix( c0.h(), c1.h(), w );
    const float S = kvs::Math::Mix( c0.s(), c1.s(), w );
    const float V = kvs::Math::Mix( c0.v(), c1.v(), w );
    return kvs::RGBColor( kvs::HSVColor( H, S, V ) );
};

}

namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 */
/*==========================================================================*/
ColorMap::ColorMap():
    m_color_space( RGBSpace ),
    m_resolution( ::Resolution ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 *  @param  resolution [in] resolution
 */
/*==========================================================================*/
ColorMap::ColorMap( const size_t resolution ):
    m_color_space( RGBSpace ),
    m_resolution( resolution ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 *  @param  table [in] opacity value table
 */
/*==========================================================================*/
ColorMap::ColorMap( const ColorMap::Table& table ):
    m_color_space( RGBSpace ),
    m_resolution( table.size() / 3 ),
    m_min_value( 0.0f ),
    m_max_value( 0.0f ),
    m_points(),
    m_table( table )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColoryMap class.
 *  @param  resolution [in] resolution
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
ColorMap::ColorMap( const size_t resolution, const float min_value, const float max_value ):
    m_color_space( RGBSpace ),
    m_resolution( resolution ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 *  @param  table [in] opacity value table
 *  @param  min_value [in] min value
 *  @param  max_value [in] max value
 */
/*==========================================================================*/
ColorMap::ColorMap( const ColorMap::Table& table, const float min_value, const float max_value ):
    m_color_space( RGBSpace ),
    m_resolution( table.size() / 3 ),
    m_min_value( min_value ),
    m_max_value( max_value ),
    m_points(),
    m_table( table )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new ColorMap class.
 *  @param  color_map [in] color map
 */
/*==========================================================================*/
ColorMap::ColorMap( const ColorMap& other ):
    m_color_space( other.m_color_space ),
    m_resolution( other.m_resolution ),
    m_min_value( other.m_min_value ),
    m_max_value( other.m_max_value ),
    m_points( other.m_points ),
    m_table( other.m_table )
{
}

/*==========================================================================*/
/**
 *  @brief  Destroys the OpacityMap class.
 */
/*==========================================================================*/
ColorMap::~ColorMap()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the range is specified.
 *  @return true (specified) or false (not specified)
 */
/*===========================================================================*/
bool ColorMap::hasRange() const
{
    return !kvs::Math::Equal( m_min_value, m_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Adds a control point as RGB color.
 *  @param  value [in] scalar value in [min_value, max_value]
 *  @param  color [in] RGB color value
 */
/*===========================================================================*/
void ColorMap::addPoint( const float value, const kvs::RGBColor color )
{
    m_points.push_back( Point( value, color ) );
}

/*===========================================================================*/
/**
 *  @brief  Adds a control point as HSV color
 *  @param  value [in] scalar value in [min_value, max_value]
 *  @param  color [in] HSV color value
 */
/*===========================================================================*/
void ColorMap::addPoint( const float value, const kvs::HSVColor color )
{
    this->addPoint( value, kvs::RGBColor( color ) );
}

/*===========================================================================*/
/**
 *  @brief  Removes the constrol point associated with the given scalar value.
 *  @param  value [in] scalar value in [min_value, max_value]
 */
/*===========================================================================*/
void ColorMap::removePoint( const float value )
{
    m_points.remove_if( ::Equal( value ) );
}

/*==========================================================================*/
/**
 *  @brief  Creates the color map.
 */
/*==========================================================================*/
void ColorMap::create()
{
    kvs::Real32 min_value = 0.0f;
    kvs::Real32 max_value = static_cast<kvs::Real32>( m_resolution - 1 );
    if ( this->hasRange() )
    {
        min_value = this->minValue();
        max_value = this->maxValue();
    }

    m_table.allocate( ::NumberOfChannels * m_resolution );
    if ( m_points.size() == 0 )
    {
        const float min_hue = 0.0f;   // blue
        const float max_hue = 240.0f; // red
        const float increment = ( max_hue - min_hue ) / static_cast<float>( m_resolution - 1 );

        kvs::UInt8* color = m_table.data();
        for ( size_t i = 0; i < m_resolution; ++i )
        {
            // HSV to RGB
            const kvs::HSVColor hsv(
                ( max_hue - increment * static_cast<float>( i ) ) / 360.0f,
                1.0f,
                1.0f );
            const kvs::RGBColor rgb( hsv );

            *( color++ ) = rgb.red();
            *( color++ ) = rgb.green();
            *( color++ ) = rgb.blue();
        }

    }
    else
    {
        m_points.sort( ::Less() );

        const kvs::RGBColor black( 0, 0, 0 );
        const kvs::RGBColor white( 255, 255, 255 );
        if ( m_points.front().first > min_value ) this->addPoint( min_value, black );
        if ( m_points.back().first < max_value ) this->addPoint( max_value, white );

        const float stride = ( max_value - min_value ) / static_cast<float>( m_resolution - 1 );
        float f = min_value;
        for ( size_t i = 0; i < m_resolution; ++i, f += stride )
        {
            Points::iterator p = m_points.begin();
            Points::iterator last = m_points.end();

            kvs::RGBColor color( 0, 0, 0 );
            Point p0( min_value, kvs::RGBColor(   0,   0,   0 ) );
            Point p1( max_value, kvs::RGBColor( 255, 255, 255 ) );
            while ( p != last )
            {
                const float s = p->first;
                if ( kvs::Math::Equal( f, s ) )
                {
                    color = p->second;
                    break;
                }
                else if ( f < s )
                {
                    p1 = *p;
                    // Interpolate.
                    const float s0 = p0.first;
                    const float s1 = p1.first;
                    if ( m_color_space == RGBSpace )
                    {
                        const kvs::RGBColor c0 = p0.second;
                        const kvs::RGBColor c1 = p1.second;
                        color = ::Interpolate( f, s0, s1, c0, c1 );
                    }
                    else if ( m_color_space == HSVSpace )
                    {
                        const kvs::HSVColor c0 = p0.second;
                        const kvs::HSVColor c1 = p1.second;
                        color = ::Interpolate( f, s0, s1, c0, c1 );
                    }
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
                            color = p0.second;
                        }
                    }
                }
            }

            m_table[ i * ::NumberOfChannels + 0 ] = color.r();
            m_table[ i * ::NumberOfChannels + 1 ] = color.g();
            m_table[ i * ::NumberOfChannels + 2 ] = color.b();
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Access operator [].
 *  @param  index [in] index of the color map.
 *  @retval RGB color value
 */
/*==========================================================================*/
const kvs::RGBColor ColorMap::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->resolution() );

    const size_t offset = ::NumberOfChannels * index;
    return kvs::RGBColor( m_table.data() + offset );
}

/*===========================================================================*/
/**
 *  @brief  Returns interpolated RGB color value by assuming piecewise linear map.
 *  @param  value [in] value
 *  @return interpolated RGB color value
 */
/*===========================================================================*/
const kvs::RGBColor ColorMap::at( const float value ) const
{
    if ( value <= m_min_value )
    {
        const kvs::RGBColor color( m_table.data() );
        return color;
    }
    else if ( value >= m_max_value )
    {
        const kvs::RGBColor color( m_table.data() + ::NumberOfChannels * ( m_resolution - 1 ) );
        return color;
    }

    const float r = static_cast<float>( m_resolution - 1 );
    const float v = ( value - m_min_value ) / ( m_max_value - m_min_value ) * r;
    const size_t s0 = static_cast<size_t>( v );
    const size_t s1 = s0 + 1;

    const kvs::RGBColor c0( m_table.data() + ::NumberOfChannels * s0 );
    const kvs::RGBColor c1( m_table.data() + ::NumberOfChannels * s1 );

    const int r0 = c0.r();
    const int g0 = c0.g();
    const int b0 = c0.b();
    const int r1 = c1.r();
    const int g1 = c1.g();
    const int b1 = c1.b();

    const kvs::UInt8 R = static_cast<kvs::UInt8>( ( r1 - r0 ) * v + r0 * s1 - r1 * s0 );
    const kvs::UInt8 G = static_cast<kvs::UInt8>( ( g1 - g0 ) * v + g0 * s1 - g1 * s0 );
    const kvs::UInt8 B = static_cast<kvs::UInt8>( ( b1 - b0 ) * v + b0 * s1 - b1 * s0 );

    return kvs::RGBColor( R, G, B );
}

/*==========================================================================*/
/**
 *  @brief  Substitution operator =.
 *  @param  opacity_map [in] color map
 *  @retval opacity map
 */
/*==========================================================================*/
ColorMap& ColorMap::operator =( const ColorMap& rhs )
{
    m_resolution = rhs.m_resolution;
    m_min_value = rhs.m_min_value;
    m_max_value = rhs.m_max_value;
    m_points = rhs.m_points;
    m_table = rhs.m_table;

    return *this;
}

} // end of namespace kvs
