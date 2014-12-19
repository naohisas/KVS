/*****************************************************************************/
/**
 *  @file   Tubeline.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tubeline.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__TUBELINE_H_INCLUDE
#define KVS__TUBELINE_H_INCLUDE

#include <kvs/LineObject>
#include <kvs/PolygonObject>
#include <kvs/Module>
#include <kvs/FilterBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Create tubeline from line object.
 */
/*===========================================================================*/
class Tubeline : public kvs::FilterBase, public kvs::PolygonObject
{
    kvsModule( kvs::Tubeline, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::PolygonObject );

protected:

    size_t m_ndivisions; ///< number of divisions of circle

public:

    Tubeline( void );
    Tubeline( const kvs::LineObject* object, const size_t ndivisions = 6 );
    virtual ~Tubeline( void );

    void setNumberOfDivisions( const size_t ndivisions );

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    void filtering_strip( const kvs::LineObject* line );
    void filtering_uniline( const kvs::LineObject* line );
    void filtering_polyline( const kvs::LineObject* line );
    void filtering_segment( const kvs::LineObject* line );

protected:

    void calculate_tubes(
        std::vector<kvs::Real32>* vertices,
        std::vector<kvs::UInt8>* colors,
        std::vector<kvs::UInt32>* connections,
        std::vector<kvs::Real32>* normals,
        const kvs::ValueArray<kvs::Real32> line_vertices,
        const kvs::ValueArray<kvs::Real32> line_sizes,
        const kvs::ValueArray<kvs::UInt8> line_colors,
        const size_t nvertices,
        const kvs::PolygonObject::ColorType color_type );

    void calculate_circles(
        std::vector<kvs::Vector3f>* start_circle,
        std::vector<kvs::Vector3f>* end_circle,
        const kvs::Vector3f& start_postion,
        const kvs::Vector3f& end_position,
        const float radius,
        const float pre_radius,
        const float post_radius );

    void set_vertices(
        std::vector<kvs::Real32>* vertices,
        const std::vector<kvs::Vector3f>& start_circle,
        const std::vector<kvs::Vector3f>& end_circle );

    void set_colors(
        std::vector<kvs::UInt8>* colors,
        const kvs::RGBColor& start_color,
        const kvs::RGBColor& end_color,
        const kvs::PolygonObject::ColorType color_type );

    void set_connections_and_normals(
        std::vector<kvs::UInt32>* connections,
        std::vector<kvs::Real32>* normals,
        const std::vector<kvs::Vector3f>& start_circle,
        const std::vector<kvs::Vector3f>& end_circle,
        const size_t vertex_number );

#if 1 // KVS_ENABLE_DEPRECATED
public:
    void setNDivisions( const size_t ndivisions ) { this->setNumberOfDivisions( ndivisions ); }
#endif
};

} // end of namespace kvs

#endif // KVS__TUBELINE_H_INCLUDE
