/****************************************************************************/
/**
 *  @file   PolygonObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonObject.h 1815 2014-10-02 01:17:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <ostream>
#include <kvs/GeometryObjectBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/RGBColor>
#include <kvs/Module>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Polygon object class.
 */
/*==========================================================================*/
class PolygonObject : public kvs::GeometryObjectBase
{
    kvsModule( kvs::PolygonObject, Object );
    kvsModuleBaseClass( kvs::GeometryObjectBase );

public:

    enum PolygonType
    {
        Triangle   = 3,
        Quadrangle = 4,
        Tri        = Triangle,
        Quad       = Quadrangle,
        UnknownPolygonType
    };

    enum ColorType
    {
        VertexColor  = 0,
        PolygonColor = 1,
        UnknownColorType
    };

    enum NormalType
    {
        VertexNormal  = 0,
        PolygonNormal = 1,
        UnknownNormalType
    };

private:

    PolygonType m_polygon_type; ///< polygon type
    ColorType m_color_type; ///< polygon color type
    NormalType m_normal_type; ///< polygon normal type
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection array
    kvs::ValueArray<kvs::UInt8> m_opacities; ///< opacity array

public:

    PolygonObject();

    void shallowCopy( const PolygonObject& object );
    void deepCopy( const PolygonObject& object );
    void clear();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    void setPolygonType( const PolygonType polygon_type ) { m_polygon_type = polygon_type; }
    void setPolygonTypeToTriangle() { this->setPolygonType( Triangle ); }
    void setPolygonTypeToQuadrangle() { this->setPolygonType( Quadrangle ); }
    void setColorType( const ColorType color_type ) { m_color_type = color_type; }
    void setColorTypeToVertex() { this->setColorType( VertexColor ); }
    void setColorTypeToPolygon() { this->setColorType( PolygonColor ); }
    void setNormalType( const NormalType normal_type ) { m_normal_type = normal_type; }
    void setNormalTypeToVertex() { this->setNormalType( VertexNormal ); }
    void setNormalTypeToPolygon() { this->setNormalType( PolygonNormal ); }
    void setConnections( const kvs::ValueArray<kvs::UInt32>& connections ) { m_connections = connections; }
    void setOpacities( const kvs::ValueArray<kvs::UInt8>& opacities ) { m_opacities = opacities; }
    void setColor( const kvs::RGBColor& color );
    void setOpacity( const kvs::UInt8 opacity );

    PolygonType polygonType() const { return m_polygon_type; }
    ColorType colorType() const { return m_color_type; }
    NormalType normalType() const { return m_normal_type; }
    size_t numberOfConnections() const;
    size_t numberOfOpacities() const { return m_opacities.size(); }
    kvs::UInt8 opacity( const size_t index = 0 ) const { return m_opacities[index]; }

    const kvs::ValueArray<kvs::UInt32>& connections() const { return m_connections; }
    const kvs::ValueArray<kvs::UInt8>& opacities() const { return m_opacities; }

public:
    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::UInt8>& opacities,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const ColorType color_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( color_type );
        this->setNormalType( normal_type );
        this->setConnections( connections );
        this->setOpacities( opacities );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::UInt8& opacity,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const ColorType color_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( color_type );
        this->setNormalType( normal_type );
        this->setConnections( connections );
        this->setOpacity( opacity );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::UInt8>& opacities,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( PolygonObject::PolygonColor );
        this->setNormalType( normal_type );
        this->setConnections( connections );
        this->setOpacities( opacities );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::RGBColor& color,
                        const kvs::UInt8& opacity,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( PolygonObject::PolygonColor );
        this->setNormalType( normal_type );
        this->setConnections( connections );
        this->setOpacity( opacity );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::UInt8>& opacities,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const ColorType color_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( color_type );
        this->setNormalType( normal_type );
        this->setOpacities( opacities );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::UInt8& opacity,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const ColorType color_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColors( colors );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( color_type );
        this->setNormalType( normal_type );
        this->setOpacity( opacity );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::UInt8>& opacities,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( PolygonObject::PolygonColor );
        this->setNormalType( normal_type );
        this->setOpacities( opacities );
    }

    KVS_DEPRECATED( PolygonObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::UInt8& opacity,
                        const kvs::ValueArray<kvs::Real32>& normals,
                        const PolygonType polygon_type,
                        const NormalType normal_type ) )
    {
        setGeometryType( Polygon );
        this->setCoords( coords );
        this->setColor( color );
        this->setNormals( normals );
        this->setPolygonType( polygon_type );
        this->setColorType( PolygonObject::PolygonColor );
        this->setNormalType( normal_type );
        this->setOpacity( opacity );
    }

    KVS_DEPRECATED( size_t nconnections() const ) { return this->numberOfConnections(); }
    KVS_DEPRECATED( size_t nopacities() const ) { return this->numberOfOpacities(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const PolygonObject& object ) );
};

} // end of namespace kvs
