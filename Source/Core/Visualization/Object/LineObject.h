/*****************************************************************************/
/**
 *  @file   LineObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineObject.h 1815 2014-10-02 01:17:28Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/GeometryObjectBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/RGBColor>
#include <kvs/Module>
#include <kvs/Deprecated>


namespace kvs
{

class PolygonObject;

/*===========================================================================*/
/**
 *  @brief  Line object class.
 */
/*===========================================================================*/
class LineObject : public kvs::GeometryObjectBase
{
    kvsModule( kvs::LineObject, Object );
    kvsModuleBaseClass( kvs::GeometryObjectBase );

public:

    enum LineType
    {
        Strip,
        Uniline,
        Polyline,
        Segment,
        UnknownLineType
    };

    enum ColorType
    {
        VertexColor,
        LineColor,
        UnknownColorType
    };

private:

    LineType m_line_type; ///< line type
    ColorType m_color_type; ///< line color type
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection array
    kvs::ValueArray<kvs::Real32> m_sizes; ///< size array

public:

    LineObject();
    explicit LineObject( const kvs::PolygonObject& polygon );

    void shallowCopy( const LineObject& object );
    void deepCopy( const LineObject& object );
    void clear();
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    void setLineType( const LineType line_type ) { m_line_type = line_type; }
    void setLineTypeToStrip() { this->setLineType( Strip ); }
    void setLineTypeToUniline() { this->setLineType( Uniline ); }
    void setLineTypeToPolyline() { this->setLineType( Polyline ); }
    void setLineTypeToSegment() { this->setLineType( Segment ); }
    void setColorType( const ColorType color_type ) { m_color_type = color_type; }
    void setColorTypeToVertex() { this->setColorType( VertexColor ); }
    void setColorTypeToLine() { this->setColorType( LineColor ); }
    void setConnections( const kvs::ValueArray<kvs::UInt32>& connections ) { m_connections = connections; }
    void setSizes( const kvs::ValueArray<kvs::Real32>& sizes ) { m_sizes = sizes; }
    void setColor( const kvs::RGBColor& color );
    void setSize( const kvs::Real32 size );

    LineType lineType() const { return m_line_type; }
    ColorType colorType() const { return m_color_type; }
    size_t numberOfConnections() const;
    size_t numberOfSizes() const;

    const kvs::Vec2ui connection( const size_t index = 0 ) const;
    kvs::Real32 size( const size_t index = 0 ) const;

    const kvs::ValueArray<kvs::UInt32>& connections() const { return m_connections; }
    const kvs::ValueArray<kvs::Real32>& sizes() const { return m_sizes; }

public:
    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::Real32>& sizes,
                        const LineType line_type,
                        const ColorType color_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColors( colors );
        this->setLineType( line_type );
        this->setColorType( color_type );
        this->setConnections( connections );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::Real32 size,
                        const LineType line_type,
                        const ColorType color_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColors( colors );
        this->setLineType( line_type );
        this->setColorType( color_type );
        this->setConnections( connections );
        this->setSize( size );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::Real32>& sizes,
                        const LineType line_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColor( color );
        this->setLineType( line_type );
        this->setColorType( LineObject::LineColor );
        this->setConnections( connections );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt32>& connections,
                        const kvs::RGBColor& color,
                        const kvs::Real32 size,
                        const LineType line_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColor( color );
        this->setLineType( line_type );
        this->setColorType( LineObject::LineColor );
        this->setConnections( connections );
        this->setSize( size );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::ValueArray<kvs::Real32>& sizes,
                        const ColorType color_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColors( colors );
        this->setLineType( LineObject::Strip );
        this->setColorType( color_type );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::ValueArray<kvs::UInt8>& colors,
                        const kvs::Real32 size,
                        const ColorType color_type ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColors( colors );
        this->setLineType( LineObject::Strip );
        this->setColorType( color_type );
        this->setSize( size );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::ValueArray<kvs::Real32>& sizes ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColor( color );
        this->setLineType( LineObject::Strip );
        this->setColorType( LineObject::LineColor );
        this->setSizes( sizes );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords,
                        const kvs::RGBColor& color,
                        const kvs::Real32 size ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColor( color );
        this->setLineType( LineObject::Strip );
        this->setColorType( LineObject::LineColor );
        this->setSize( size );
    }

    KVS_DEPRECATED( LineObject(
                        const kvs::ValueArray<kvs::Real32>& coords ) )
    {
        setGeometryType( Line );
        this->setCoords( coords );
        this->setColor( kvs::RGBColor( 255, 255, 255 ) );
        this->setLineType( LineObject::Strip );
        this->setColorType( LineObject::LineColor );
        this->setSize( 1.0f );
    }

    KVS_DEPRECATED( size_t nconnections() const ) { return this->numberOfConnections(); }
    KVS_DEPRECATED( size_t nsizes() const ) { return this->numberOfSizes(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const LineObject& object ) );
};

} // end of namespace kvs
