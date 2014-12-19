/*****************************************************************************/
/**
 *  @file   Gis.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__GIS_H_INCLUDE
#define KVS__GIS_H_INCLUDE

#include <iostream>
#include <vector>
#include <kvs/FileFormatBase>
#include <kvs/Vector2>
#include <kvs/RGBColor>
#include <kvs/Indent>
#include "Mesh.h"
#include "Area.h"
#include "OceanValue.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  GIS (Geographical Information System) class.
 */
/*===========================================================================*/
class Gis : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

private:

    std::vector<kvs::gis::Mesh*> m_meshes; ///< mesh list
    kvs::gis::Area m_area; ///< area of all meshes
    kvs::gis::Area m_render_area; ///< render area of the mesh
    kvs::RGBColor m_ocean_color; ///< ocean color

public:

    static bool CheckExtension( const std::string& filename );

public:

    Gis();
    Gis(
        const std::string& filename,
        const kvs::gis::Area& render_area = kvs::gis::Area( 0.0f, 999.0f, 0.0f, 999.0f ) );
    virtual ~Gis();

    const kvs::gis::Area& area() const;
    const kvs::gis::Area& renderArea() const;
    const std::vector<kvs::gis::Mesh*>& meshes() const;
    const kvs::gis::Mesh* mesh( const size_t index ) const;
    kvs::RGBColor oceanColor() const;

    void setRenderArea( const kvs::gis::Area& render_area );
    void setOceanColor( const kvs::RGBColor& color );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filenames );

private:

    bool read_mesh( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs

#endif // KVS__GIS_H_INCLUDE
