/****************************************************************************/
/**
 *  @file   OpacityMap.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpacityMap.h 1619 2013-07-18 05:12:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__OPACITY_MAP_H_INCLUDE
#define KVS__OPACITY_MAP_H_INCLUDE

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

    typedef kvs::ValueArray<float> Table;
    typedef std::pair<float,float> Point;
    typedef std::list<Point> Points;

private:

    size_t m_resolution; ///< table resolution
    float m_min_value; ///< min. value
    float m_max_value; ///< max. value
    Points m_points; ///< control point list
    Table m_table; ///< value table

public:

    OpacityMap();
    explicit OpacityMap( const size_t resolution );
    explicit OpacityMap( const Table& table );
    OpacityMap( const OpacityMap& other );
    OpacityMap( const size_t resolution, const float min_value, const float max_value );
    OpacityMap( const Table& table, const float min_value, const float max_value );
    virtual ~OpacityMap() {}

    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }
    size_t resolution() const { return m_resolution; }
    const Points& points() const { return m_points; }
    const Table& table() const { return m_table; }

    bool hasRange() const;
    void setRange( const float min_value, const float max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setResolution( const size_t resolution ) { m_resolution = resolution; }
    void addPoint( const float value, const float opacity );
    void removePoint( const float value );
    void create();

    kvs::Real32 operator []( const size_t index ) const;
    kvs::Real32 at( const float value ) const;
    OpacityMap& operator =( const OpacityMap& rhs );
};

} // end of namespace kvs

#endif // KVS__OPACITY_MAP_H_INCLUDE
