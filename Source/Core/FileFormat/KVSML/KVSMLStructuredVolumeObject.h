/****************************************************************************/
/**
 *  @file   KVSMLStructuredVolumeObject.h
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


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSML structured volume object format.
 */
/*===========================================================================*/
class KVSMLStructuredVolumeObject : public kvs::FileFormatBase
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
    WritingDataType m_writing_type; ///< writing data type
    std::string m_grid_type; ///< grid type
    bool m_has_label; ///< data label is specified or not
    bool m_has_unit; ///< data unit is specified or not
    bool m_has_min_value; ///< min. value is specified or not
    bool m_has_max_value; ///< max. value is specified or not
    std::string m_label; ///< data label
    std::string m_unit; ///< data unit
    size_t m_veclen; ///< vector length
    kvs::Vec3ui m_resolution; ///< grid resolution
    double m_min_value; ///< min. value
    double m_max_value; ///< max. value
    kvs::AnyValueArray m_values; ///< field value array
    kvs::ValueArray<float> m_coords; ///< coordinate array

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLStructuredVolumeObject();
    KVSMLStructuredVolumeObject( const std::string& filename );

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    const kvs::kvsml::ObjectTag& objectTag() const { return m_object_tag; }
    const std::string& gridType() const { return m_grid_type; }
    bool hasLabel() const { return m_has_label; }
    bool hasUnit() const { return m_has_unit; }
    bool hasMinValue() const { return m_has_min_value; }
    bool hasMaxValue() const { return m_has_max_value; }
    bool hasObjectCoord() const { return m_object_tag.hasObjectCoord(); }
    bool hasExternalCoord() const { return m_object_tag.hasExternalCoord(); }
    const std::string& label() const { return m_label; }
    const std::string& unit() const { return m_unit; }
    size_t veclen() const { return m_veclen; }
    const kvs::Vec3ui& resolution() const { return m_resolution; }
    double minValue() const { return m_min_value; }
    double maxValue() const { return m_max_value; }
    const kvs::Vec3& minObjectCoord() const { return m_object_tag.minObjectCoord(); }
    const kvs::Vec3& maxObjectCoord() const { return m_object_tag.maxObjectCoord(); }
    const kvs::Vec3& minExternalCoord() const { return m_object_tag.minExternalCoord(); }
    const kvs::Vec3& maxExternalCoord() const { return m_object_tag.maxExternalCoord(); }
    const kvs::AnyValueArray& values() const { return m_values; }
    const kvs::ValueArray<float>& coords() const { return m_coords; }

    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setWritingDataTypeToAscii() { this->setWritingDataType( Ascii ); }
    void setWritingDataTypeToExternalAscii() { this->setWritingDataType( ExternalAscii ); }
    void setWritingDataTypeToExternalBinary() { this->setWritingDataType( ExternalBinary ); }
    void setGridType( const std::string& type ) { m_grid_type = type; }
    void setLabel( const std::string& label ) { m_has_label = true; m_label = label; }
    void setUnit( const std::string& unit ) { m_has_unit = true; m_unit = unit; }
    void setVeclen( const size_t veclen ) { m_veclen = veclen; }
    void setResolution( const kvs::Vec3ui& resolution ) { m_resolution = resolution; }
    void setMinValue( const double value ) { m_has_min_value = true; m_min_value = value; }
    void setMaxValue( const double value ) { m_has_max_value = true; m_max_value = value; }
    void setValues( const kvs::AnyValueArray& values ) { m_values = values; }
    void setCoords( const kvs::ValueArray<float>& coords ) { m_coords = coords; }
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
