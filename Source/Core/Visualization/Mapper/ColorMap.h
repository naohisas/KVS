/****************************************************************************/
/**
 *  @file   ColorMap.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorMap.h 1756 2014-05-02 02:28:45Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <list>
#include <utility>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/RGBColor>
#include <kvs/HSVColor>


namespace kvs
{

/*==========================================================================*/
/**
 *  Color map class.
 */
/*==========================================================================*/
class ColorMap
{
public:
    static kvs::ColorMap CoolWarm( const size_t resolution = 256 );
    static kvs::ColorMap BrewerBrBG( const size_t resolution = 256 );
    static kvs::ColorMap BrewerPiYG( const size_t resolution = 256 );
    static kvs::ColorMap BrewerPRGn( const size_t resolution = 256 );
    static kvs::ColorMap BrewerPuOr( const size_t resolution = 256 );
    static kvs::ColorMap BrewerRdBu( const size_t resolution = 256 );
    static kvs::ColorMap BrewerRdGy( const size_t resolution = 256 );
    static kvs::ColorMap BrewerRdYlBu( const size_t resolution = 256 );
    static kvs::ColorMap BrewerRdYlGn( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSpectral( const size_t resolution = 256 );

public:
    typedef kvs::ValueArray<kvs::UInt8> Table;
    typedef std::pair<float,kvs::RGBColor> Point;
    typedef std::list<Point> Points;

    enum ColorSpace
    {
        RGBSpace,
        HSVSpace,
        LabSpace,
        MshSpace
    };

private:
    ColorSpace m_color_space; ///< color space for interpolation
    size_t m_resolution; ///< table resolution
    float m_min_value; ///< min. value
    float m_max_value; ///< max. value
    Points m_points; ///< control point list
    Table m_table; ///< value table

public:
    ColorMap();
    explicit ColorMap( const size_t resolution );
    explicit ColorMap( const Table& table );
    ColorMap( const ColorMap& other );
    ColorMap( const size_t resolution, const float min_value, const float max_value );
    ColorMap( const Table& table, const float min_value, const float max_value );
    virtual ~ColorMap() {}

    ColorSpace colorSpace() const { return m_color_space; }
    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }
    size_t resolution() const { return m_resolution; }
    const Points& points() const { return m_points; }
    const Table& table() const { return m_table; }
    bool hasRange() const;

    void setColorSpace( const ColorSpace space ) { m_color_space = space; }
    void setColorSpaceToRGB() { this->setColorSpace( RGBSpace ); }
    void setColorSpaceToHSV() { this->setColorSpace( HSVSpace ); }
    void setColorSpaceToLab() { this->setColorSpace( LabSpace ); }
    void setColorSpaceToMsh() { this->setColorSpace( MshSpace ); }
    void setRange( const float min_value, const float max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setResolution( const size_t resolution ) { m_resolution = resolution; }
    void addPoint( const float value, const kvs::RGBColor color );
    void addPoint( const float value, const kvs::HSVColor color );
    void removePoint( const float value );
    void clearPoints();
    void reversePoints();
    void create();

    const kvs::RGBColor operator []( const size_t index ) const;
    const kvs::RGBColor at( const float value ) const;
    ColorMap& operator =( const ColorMap& rhs );
};

} // end of namespace kvs
