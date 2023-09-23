/****************************************************************************/
/**
 *  @file   ColorMap.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <list>
#include <utility>
#include <functional>
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
    using ColorMapFunction = std::function<kvs::ColorMap(const size_t)>;
    static void SetDefaultColorMap( ColorMapFunction func );
    static kvs::ColorMap DefaultColorMap( const size_t resolution = 256 );

    static kvs::ColorMap Rainbow( const size_t resolution = 256 );
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
    static kvs::ColorMap BrewerSingleHueGreys( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSingleHueGreens( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSingleHueBlues( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSingleHueReds( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSingleHuePurples( const size_t resolution = 256 );
    static kvs::ColorMap BrewerSingleHueOranges( const size_t resolution = 256 );
    static kvs::ColorMap Viridis( const size_t resolution = 256 );
    static kvs::ColorMap Plasma( const size_t resolution = 256 );
    static kvs::ColorMap Inferno( const size_t resolution = 256 );
    static kvs::ColorMap Magma( const size_t resolution = 256 );
    static kvs::ColorMap Cividis( const size_t resolution = 256 );

public:
    using Table = kvs::ValueArray<kvs::UInt8>;
    using Point = std::pair<float,kvs::RGBColor>;
    using Points = std::list<Point>;

    enum ColorSpace
    {
        RGBSpace,
        HCLSpace,
        HSLSpace,
        HSVSpace,
        LabSpace,
        MshSpace
    };

private:
    ColorSpace m_color_space = RGBSpace; ///< color space for interpolation
    size_t m_resolution = 256; ///< table resolution
    float m_min_value = 0.0f; ///< min. value
    float m_max_value = 0.0f; ///< max. value
    Points m_points{}; ///< control point list
    Table m_table{}; ///< value table

public:
    ColorMap() = default;
    explicit ColorMap( const size_t resolution ): m_resolution( resolution ) {}
    explicit ColorMap( const Table& table ): m_resolution( table.size() / 3 ), m_table( table ) {}
    ColorMap( const ColorMap& other ) { *this = other; }
    ColorMap( const size_t resolution, const float min_value, const float max_value );
    ColorMap( const Table& table, const float min_value, const float max_value );
    ColorMap( const size_t resolution, const Points& points );
    ColorMap( const size_t resolution, const Points& points, const float min_value, const float max_value );
    ColorMap( const size_t resolution, const std::list<kvs::RGBColor>& colors );
    virtual ~ColorMap() = default;

    ColorSpace colorSpace() const { return m_color_space; }
    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }
    size_t resolution() const { return m_resolution; }
    const Points& points() const { return m_points; }
    const Table& table() const { return m_table; }
    bool hasRange() const;

    void setColorSpace( const ColorSpace space ) { m_color_space = space; }
    void setColorSpaceToRGB() { this->setColorSpace( RGBSpace ); }
    void setColorSpaceToHCL() { this->setColorSpace( HCLSpace ); }
    void setColorSpaceToHSL() { this->setColorSpace( HSLSpace ); }
    void setColorSpaceToHSV() { this->setColorSpace( HSVSpace ); }
    void setColorSpaceToLab() { this->setColorSpace( LabSpace ); }
    void setColorSpaceToMsh() { this->setColorSpace( MshSpace ); }
    void setRange( const float min_value, const float max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setResolution( const size_t resolution ) { m_resolution = resolution; }
    void addPoint( const float value, const kvs::RGBColor color );
    void addPoint( const float value, const kvs::HSVColor color );
    void setPoints( const Points& points ) { m_points = points; }
    void setPoints( const std::list<kvs::RGBColor>& colors );
    void removePoint( const float value );
    void clearPoints() { m_points.clear(); }
    void reversePoints() { m_points.reverse(); }
    void create();

    const kvs::RGBColor operator []( const size_t index ) const;
    const kvs::RGBColor at( const float value ) const;
    ColorMap& operator =( const ColorMap& rhs );
};

} // end of namespace kvs
