/****************************************************************************/
/**
 *  @file   KVSMLUnstructuredVolumeObject.h
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
/****************************************************************************/
#pragma once
#include <kvs/FileFormatBase>
#include <kvs/AnyValueArray>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Indent>
#include <string>
#include "KVSMLTag.h"
#include "ObjectTag.h"
#include "UnstructuredVolumeObjectTag.h"
#include "NodeTag.h"
#include "ValueTag.h"
#include "CoordTag.h"
#include "CellTag.h"
#include "ConnectionTag.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSML unstructured volume object format.
 */
/*===========================================================================*/
class KVSMLUnstructuredVolumeObject : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;

    enum WritingDataType
    {
        Ascii = 0,
        ExternalAscii,
        ExternalBinary
    };

private:
    kvs::kvsml::KVSMLTag m_kvsml_tag; ///< KVSML tag information
    kvs::kvsml::ObjectTag m_object_tag; ///< Object tag information
    kvs::kvsml::UnstructuredVolumeObjectTag m_volume_tag; ///< Unstructured volume object tag information
    kvs::kvsml::NodeTag m_node_tag; ///< Node tag information
    kvs::kvsml::ValueTag m_value_tag; ///< Value tag information
    kvs::kvsml::CoordTag m_coord_tag; ///< Coord tag information
    kvs::kvsml::CellTag m_cell_tag; ///< Cell tag information
    kvs::kvsml::ConnectionTag m_connection_tag; ///< Connection tag information
    WritingDataType m_writing_type; ///< writing data type
    kvs::AnyValueArray m_values; ///< field value array
    kvs::ValueArray<kvs::Real32> m_coords; ///< coordinate value array
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection id array

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLUnstructuredVolumeObject();
    KVSMLUnstructuredVolumeObject( const std::string& filename );

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    const kvs::kvsml::ObjectTag& objectTag() const { return m_object_tag; }
    const std::string& cellType() const { return m_volume_tag.cellType(); }
    bool hasLabel() const { return m_value_tag.hasLabel(); }
    bool hasUnit() const { return m_value_tag.hasUnit(); }
    bool hasMinValue() const { return m_value_tag.hasMinValue(); }
    bool hasMaxValue() const { return m_value_tag.hasMaxValue(); }
    bool hasObjectCoord() const { return m_object_tag.hasObjectCoord(); }
    bool hasExternalCoord() const { return m_object_tag.hasExternalCoord(); }
    const std::string& label() const { return m_value_tag.label(); }
    const std::string& unit() const { return m_value_tag.unit(); }
    size_t veclen() const { return m_value_tag.veclen(); }
    size_t nnodes() const { return m_node_tag.nnodes(); }
    size_t ncells() const { return m_cell_tag.ncells(); }
    double minValue() const { return m_value_tag.minValue(); }
    double maxValue() const { return m_value_tag.maxValue(); }
    const kvs::Vec3& minObjectCoord() const { return m_object_tag.minObjectCoord(); }
    const kvs::Vec3& maxObjectCoord() const { return m_object_tag.maxObjectCoord(); }
    const kvs::Vec3& minExternalCoord() const { return m_object_tag.minExternalCoord(); }
    const kvs::Vec3& maxExternalCoord() const { return m_object_tag.maxExternalCoord(); }
    const kvs::AnyValueArray& values() const { return m_values; }
    const kvs::ValueArray<kvs::Real32>& coords() const { return m_coords; }
    const kvs::ValueArray<kvs::UInt32>& connections() const { return m_connections; }

    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setWritingDataTypeToAscii() { this->setWritingDataType( Ascii ); }
    void setWritingDataTypeToExternalAscii() { this->setWritingDataType( ExternalAscii ); }
    void setWritingDataTypeToExternalBinary() { this->setWritingDataType( ExternalBinary ); }
    void setCellType( const std::string& type ) { m_volume_tag.setCellType( type ); }
    void setLabel( const std::string& label ) { m_value_tag.setLabel( label ); }
    void setUnit( const std::string& unit ) { m_value_tag.setUnit( unit ); }
    void setVeclen( const size_t veclen ) { m_value_tag.setVeclen( veclen ); }
    void setNNodes( const size_t nnodes ) { m_node_tag.setNNodes( nnodes ); }
    void setNCells( const size_t ncells ) { m_cell_tag.setNCells( ncells ); }
    void setMinValue( const double value ) { m_value_tag.setMinValue( value ); }
    void setMaxValue( const double value ) { m_value_tag.setMaxValue( value ); }
    void setValues( const kvs::AnyValueArray& values ) { m_values = values; }
    void setCoords( const kvs::ValueArray<kvs::Real32>& coords ) { m_coords = coords; }
    void setConnections( const kvs::ValueArray<kvs::UInt32>& connections ) { m_connections = connections; }
    void setMinMaxObjectCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord )
    {
        m_object_tag.setMinMaxObjectCoords( min_coord, max_coord );
    }
    void setMinMaxExternalCoords( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord )
    {
        m_object_tag.setMinMaxExternalCoords( min_coord, max_coord );
    }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs
