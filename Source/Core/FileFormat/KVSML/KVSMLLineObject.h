/****************************************************************************/
/**
 *  @file   KVSMLLineObject.h
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
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Indent>
#include "KVSMLTag.h"
#include "ObjectTag.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSML line object format.
 */
/*===========================================================================*/
class KVSMLLineObject : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;

    enum WritingDataType
    {
        Ascii = 0,     ///< ascii data type
        ExternalAscii, ///< external ascii data type
        ExternalBinary ///< external binary data type
    };

private:
    kvs::kvsml::KVSMLTag m_kvsml_tag; ///< KVSML tag information
    kvs::kvsml::ObjectTag m_object_tag; ///< Object tag information
    WritingDataType m_writing_type; ///< writing data type
    std::string m_line_type; ///< line type
    std::string m_color_type; ///< line color type
    kvs::ValueArray<kvs::Real32> m_coords; ///< coordinate array
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection array
    kvs::ValueArray<kvs::UInt8> m_colors; ///< color (r,g,b) array
    kvs::ValueArray<kvs::Real32> m_sizes; ///< size array

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLLineObject();
    KVSMLLineObject( const std::string& filename );

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    const kvs::kvsml::ObjectTag& objectTag() const { return m_object_tag; }
    const std::string& lineType() const { return m_line_type; }
    const std::string& colorType() const { return m_color_type; }
    const kvs::ValueArray<kvs::Real32>& coords() const { return m_coords; }
    const kvs::ValueArray<kvs::UInt8>& colors() const { return m_colors; }
    const kvs::ValueArray<kvs::UInt32>& connections() const { return m_connections; }
    const kvs::ValueArray<kvs::Real32>& sizes() const { return m_sizes; }
    bool hasObjectCoord() const { return m_object_tag.hasObjectCoord(); }
    bool hasExternalCoord() const { return m_object_tag.hasExternalCoord(); }
    const kvs::Vec3& minObjectCoord() const { return m_object_tag.minObjectCoord(); }
    const kvs::Vec3& maxObjectCoord() const { return m_object_tag.maxObjectCoord(); }
    const kvs::Vec3& minExternalCoord() const { return m_object_tag.minExternalCoord(); }
    const kvs::Vec3& maxExternalCoord() const { return m_object_tag.maxExternalCoord(); }

    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setWritingDataTypeToAscii() { this->setWritingDataType( Ascii ); }
    void setWritingDataTypeToExternalAscii() { this->setWritingDataType( ExternalAscii ); }
    void setWritingDataTypeToExternalBinary() { this->setWritingDataType( ExternalBinary ); }
    void setLineType( const std::string& type ) { m_line_type = type; }
    void setColorType( const std::string& type ) { m_color_type = type; }
    void setCoords( const kvs::ValueArray<kvs::Real32>& coords ) { m_coords = coords; }
    void setColors( const kvs::ValueArray<kvs::UInt8>& colors ) { m_colors = colors; }
    void setConnections( const kvs::ValueArray<kvs::UInt32>& connections ) { m_connections = connections; }
    void setSizes( const kvs::ValueArray<kvs::Real32>& sizes ) { m_sizes = sizes; }
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
