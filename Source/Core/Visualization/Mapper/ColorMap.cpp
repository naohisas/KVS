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
#include <kvs/LabColor>
#include <kvs/MshColor>
#include <kvs/Math>


namespace
{

// Default values.
const size_t Resolution = 256;
const size_t NumberOfChannels = 3;

 /*===========================================================================*/
 /**
  *  @brief  Returns a piecewise-linearly interpolated color map.
  *  @param  colors [in] color list
  *  @param  resolution [in] resolution of the color map
  *  @param  space [in] interpolation color space
  *  @return color map interpolated in the specified color space
  */
 /*===========================================================================*/
inline kvs::ColorMap Interpolate(
    const std::list<kvs::RGBColor>& colors,
    const size_t resolution,
    kvs::ColorMap::ColorSpace space )
{
    const size_t ncolors = colors.size();
    const float stride = 1.0f / ( ncolors - 1 );

    kvs::ColorMap cmap( resolution );
    cmap.addPoint( 0.0f, colors.front() ); // start point
    auto color = colors.begin(); color++;
    auto end = colors.end(); end--;
    size_t index = 1;
    while ( color != end )
    {
        const float value = kvs::Math::Round( resolution * stride * index );
        cmap.addPoint( value, *color );
        color++; index++;
    }
    cmap.addPoint( float( resolution - 1 ), colors.back() ); // end point
    cmap.setColorSpace( space );
    cmap.create();

    return cmap;
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns cool-warm diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return cool-warm colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Kenneth Moreland, "Diverging Color Maps for Scientific Visualization",
 *      In Proceedings of the 5th International Symposium on Visual Computing,
 *      December 2009. DOI: 10.1007/978-3-642-10520-3_9.
 *      https://www.kennethmoreland.com/color-maps/
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::CoolWarm( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor(  59, 76, 192 ) );
    colors.push_back( kvs::RGBColor( 180,  4,  38 ) );
    return ::Interpolate( colors, resolution, kvs::ColorMap::MshSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer BrBG (brown, (white), blue-green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return BrBG diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerBrBG( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor(  84,  48,   5 ) );
    colors.push_back( kvs::RGBColor( 140,  81,  10 ) );
    colors.push_back( kvs::RGBColor( 191, 129,  45 ) );
    colors.push_back( kvs::RGBColor( 223, 194, 125 ) );
    colors.push_back( kvs::RGBColor( 246, 232, 195 ) );
    colors.push_back( kvs::RGBColor( 245, 245, 245 ) );
    colors.push_back( kvs::RGBColor( 199, 234, 229 ) );
    colors.push_back( kvs::RGBColor( 128, 205, 193 ) );
    colors.push_back( kvs::RGBColor(  53, 151, 143 ) );
    colors.push_back( kvs::RGBColor(   1, 102,  94 ) );
    colors.push_back( kvs::RGBColor(   0,  60,  48 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PiYG (pink, (white), yello-green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PiYG diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPiYG( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 142,   1,  82 ) );
    colors.push_back( kvs::RGBColor( 197,  27, 125 ) );
    colors.push_back( kvs::RGBColor( 222, 119, 174 ) );
    colors.push_back( kvs::RGBColor( 241, 182, 218 ) );
    colors.push_back( kvs::RGBColor( 253, 224, 239 ) );
    colors.push_back( kvs::RGBColor( 247, 247, 247 ) );
    colors.push_back( kvs::RGBColor( 230, 245, 208 ) );
    colors.push_back( kvs::RGBColor( 184, 225, 134 ) );
    colors.push_back( kvs::RGBColor( 127, 188, 134 ) );
    colors.push_back( kvs::RGBColor(  77, 146,  33 ) );
    colors.push_back( kvs::RGBColor(  39, 100,  25 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PRGn (purple, (white), green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PRGn diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPRGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor(  64,   0,  75 ) );
    colors.push_back( kvs::RGBColor( 118,  42, 131 ) );
    colors.push_back( kvs::RGBColor( 153, 112, 171 ) );
    colors.push_back( kvs::RGBColor( 194, 165, 207 ) );
    colors.push_back( kvs::RGBColor( 231, 212, 232 ) );
    colors.push_back( kvs::RGBColor( 247, 247, 247 ) );
    colors.push_back( kvs::RGBColor( 217, 240, 211 ) );
    colors.push_back( kvs::RGBColor( 166, 219, 160 ) );
    colors.push_back( kvs::RGBColor(  90, 174,  97 ) );
    colors.push_back( kvs::RGBColor(  27, 120,  55 ) );
    colors.push_back( kvs::RGBColor(   0,  68,  27 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer PuOr (orange, (white), purple) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return PuOr diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerPuOr( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 127,  59,   8 ) );
    colors.push_back( kvs::RGBColor( 179,  88,   6 ) );
    colors.push_back( kvs::RGBColor( 224, 130,  20 ) );
    colors.push_back( kvs::RGBColor( 253, 184,  99 ) );
    colors.push_back( kvs::RGBColor( 254, 224, 182 ) );
    colors.push_back( kvs::RGBColor( 247, 247, 247 ) );
    colors.push_back( kvs::RGBColor( 216, 218, 235 ) );
    colors.push_back( kvs::RGBColor( 178, 171, 210 ) );
    colors.push_back( kvs::RGBColor( 128, 115, 172 ) );
    colors.push_back( kvs::RGBColor(  84,  39, 136 ) );
    colors.push_back( kvs::RGBColor(  45,   0,  75 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdBu (red, (white), blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdBu diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 103,   0,  31 ) );
    colors.push_back( kvs::RGBColor( 178,  24,  43 ) );
    colors.push_back( kvs::RGBColor( 214,  96,  77 ) );
    colors.push_back( kvs::RGBColor( 244, 165, 130 ) );
    colors.push_back( kvs::RGBColor( 253, 219, 199 ) );
    colors.push_back( kvs::RGBColor( 247, 247, 247 ) );
    colors.push_back( kvs::RGBColor( 209, 229, 240 ) );
    colors.push_back( kvs::RGBColor( 146, 197, 222 ) );
    colors.push_back( kvs::RGBColor(  67, 147, 195 ) );
    colors.push_back( kvs::RGBColor(  33, 102, 172 ) );
    colors.push_back( kvs::RGBColor(   5,  48,  97 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdGy (red, (white), grey) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdGy diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdGy( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 103,   0,  31 ) );
    colors.push_back( kvs::RGBColor( 178,  24,  43 ) );
    colors.push_back( kvs::RGBColor( 214,  96,  77 ) );
    colors.push_back( kvs::RGBColor( 244, 165, 130 ) );
    colors.push_back( kvs::RGBColor( 253, 219, 199 ) );
    colors.push_back( kvs::RGBColor( 255, 255, 255 ) );
    colors.push_back( kvs::RGBColor( 224, 224, 224 ) );
    colors.push_back( kvs::RGBColor( 186, 186, 186 ) );
    colors.push_back( kvs::RGBColor( 135, 135, 135 ) );
    colors.push_back( kvs::RGBColor(  77,  77,  77 ) );
    colors.push_back( kvs::RGBColor(  26,  26,  26 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdYlBu (red, yello, blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdYlBu diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdYlBu( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 165,   0,  38 ) );
    colors.push_back( kvs::RGBColor( 215,  48,  39 ) );
    colors.push_back( kvs::RGBColor( 244, 109,  67 ) );
    colors.push_back( kvs::RGBColor( 253, 174,  97 ) );
    colors.push_back( kvs::RGBColor( 254, 224, 144 ) );
    colors.push_back( kvs::RGBColor( 255, 255, 191 ) );
    colors.push_back( kvs::RGBColor( 224, 243, 248 ) );
    colors.push_back( kvs::RGBColor( 171, 217, 233 ) );
    colors.push_back( kvs::RGBColor( 116, 173, 209 ) );
    colors.push_back( kvs::RGBColor(  69, 117, 180 ) );
    colors.push_back( kvs::RGBColor(  49,  54, 149 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer RdYlGn (red, yello, green) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return RdYlGn diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerRdYlGn( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 165,   0,  38 ) );
    colors.push_back( kvs::RGBColor( 215,  48,  39 ) );
    colors.push_back( kvs::RGBColor( 244, 109,  67 ) );
    colors.push_back( kvs::RGBColor( 253, 174,  97 ) );
    colors.push_back( kvs::RGBColor( 254, 224, 139 ) );
    colors.push_back( kvs::RGBColor( 255, 255, 191 ) );
    colors.push_back( kvs::RGBColor( 217, 239, 139 ) );
    colors.push_back( kvs::RGBColor( 166, 217, 106 ) );
    colors.push_back( kvs::RGBColor( 102, 189,  99 ) );
    colors.push_back( kvs::RGBColor(  26, 152,  80 ) );
    colors.push_back( kvs::RGBColor(   0, 104,  55 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

/*===========================================================================*/
/**
 *  @brief  Returns ColorBrewer spectral (red, orange, yello, green, blue) diverging colormap.
 *  @param  resolution [in] table resolution
 *  @return spectral diverging colormap
 */
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] C.A.Brewer, "Color use guidelines for mapping", visualization in modern
 *      cartography, pp.123–148, 1994. DOI: 10.1117/12.175328
 *      http://colorbrewer2.org
 */
/*===========================================================================*/
kvs::ColorMap ColorMap::BrewerSpectral( const size_t resolution )
{
    std::list<kvs::RGBColor> colors;
    colors.push_back( kvs::RGBColor( 158,   1,  66 ) );
    colors.push_back( kvs::RGBColor( 213,  62,  79 ) );
    colors.push_back( kvs::RGBColor( 244, 109,  67 ) );
    colors.push_back( kvs::RGBColor( 253, 174,  97 ) );
    colors.push_back( kvs::RGBColor( 254, 224, 139 ) );
    colors.push_back( kvs::RGBColor( 255, 255, 191 ) );
    colors.push_back( kvs::RGBColor( 230, 245, 152 ) );
    colors.push_back( kvs::RGBColor( 171, 221, 164 ) );
    colors.push_back( kvs::RGBColor( 102, 194, 165 ) );
    colors.push_back( kvs::RGBColor(  50, 136, 189 ) );
    colors.push_back( kvs::RGBColor(  94,  79, 162 ) );
    colors.reverse();
    return ::Interpolate( colors, resolution, kvs::ColorMap::LabSpace );
}

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
    m_points.remove_if( [ value ]( Point& p ) { return kvs::Math::Equal( p.first, value ); } );
}

/*===========================================================================*/
/**
 *  @brief  Clears all of the control points.
 */
/*===========================================================================*/
void ColorMap::clearPoints()
{
    m_points.clear();
}

/*===========================================================================*/
/**
 *  @brief  Reverses the control points.
 */
/*===========================================================================*/
void ColorMap::reversePoints()
{
    m_points.reverse();
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

            *( color++ ) = rgb.r();
            *( color++ ) = rgb.g();
            *( color++ ) = rgb.b();
        }
    }

    else
    {
        m_points.sort( [] ( const Point& p1, const Point& p2 ) { return p1.first < p2.first; } );

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
                    const float ratio = ( f - s0 ) / ( s1 - s0 );
                    if ( m_color_space == RGBSpace )
                    {
                        const kvs::RGBColor c0 = p0.second;
                        const kvs::RGBColor c1 = p1.second;
                        color = kvs::RGBColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == HSVSpace )
                    {
                        const kvs::HSVColor c0 = p0.second;
                        const kvs::HSVColor c1 = p1.second;
                        color = kvs::HSVColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == LabSpace )
                    {
                        const kvs::LabColor c0 = p0.second;
                        const kvs::LabColor c1 = p1.second;
                        color = kvs::LabColor::Mix( c0, c1, ratio );
                    }
                    else if ( m_color_space == MshSpace )
                    {
                        const kvs::MshColor c0 = p0.second;
                        const kvs::MshColor c1 = p1.second;
                        color = kvs::MshColor::Mix( c0, c1, ratio );
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
    const size_t s1 = kvs::Math::Min( s0 + 1, m_resolution - 1 );

    const kvs::RGBColor c0( m_table.data() + ::NumberOfChannels * s0 );
    const kvs::RGBColor c1( m_table.data() + ::NumberOfChannels * s1 );
    return kvs::RGBColor::Mix( c0, c1, v - s0 );
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
