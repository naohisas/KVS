/****************************************************************************/
/**
 *  @file   OpacityMap.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <list>
#include <utility>
#include <kvs/ValueArray>
#include <kvs/Type>


namespace kvs
{

/*==========================================================================*/
/**
 *  OpacityMap.
 */
/*==========================================================================*/
class OpacityMap
{
public:
    using Table = kvs::ValueArray<float>;
    using Point = std::pair<float,float>;
    using Points = std::list<Point>;

private:
    size_t m_resolution = 256; ///< table resolution
    float m_min_value = 0.0f; ///< min. value
    float m_max_value = 0.0f; ///< max. value
    Points m_points{}; ///< control point list
    Table m_table{}; ///< value table

public:
    OpacityMap() = default;
    explicit OpacityMap( const size_t resolution ): m_resolution( resolution ) {}
    explicit OpacityMap( const Table& table ): m_resolution( table.size() ), m_table( table ) {}
    OpacityMap( const OpacityMap& other ) { *this = other; }
    OpacityMap( const size_t resolution, const float min_value, const float max_value );
    OpacityMap( const Table& table, const float min_value, const float max_value );
    virtual ~OpacityMap() = default;

    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }
    size_t resolution() const { return m_resolution; }
    const Points& points() const { return m_points; }
    const Table& table() const { return m_table; }
    bool hasRange() const;

    void setRange( const float min_value, const float max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setResolution( const size_t resolution ) { m_resolution = resolution; }
    void setPoints( const Points& points ) { m_points = points; }
    void setPoints( const std::list<float>& opacities );
    void addPoint( const float value, const float opacity );
    void removePoint( const float value );
    void clearPoints() { m_points.clear(); }
    void reversePoints() { m_points.reverse(); }
    void create();

    kvs::Real32 operator []( const size_t index ) const;
    kvs::Real32 at( const float value ) const;
    OpacityMap& operator =( const OpacityMap& rhs );
};

} // end of namespace kvs
