/*****************************************************************************/
/**
 *  @file   KVSMLTableObject.h
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
#pragma once
#include <kvs/FileFormatBase>
#include <kvs/AnyValueArray>
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Indent>
#include <string>
#include <vector>
#include <Core/FileFormat/KVSML/KVSMLTag.h>
#include <Core/FileFormat/KVSML/ObjectTag.h>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSML table object format.
 */
/*===========================================================================*/
class KVSMLTableObject : public kvs::FileFormatBase
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
    size_t m_nrows; ///< number of rows
    size_t m_ncolumns; ///< number of columns
    std::vector<std::string> m_labels; ///< column label list
    std::vector<kvs::AnyValueArray> m_columns; ///< column list
    std::vector<bool> m_has_min_values; ///< min. value flag list
    std::vector<bool> m_has_max_values; ///< max. value flag list
    std::vector<bool> m_has_min_ranges; ///< min. range flag list
    std::vector<bool> m_has_max_ranges; ///< max. range flag list
    std::vector<double> m_min_values; ///< min. value list
    std::vector<double> m_max_values; ///< max. value list
    std::vector<double> m_min_ranges; ///< min. range list
    std::vector<double> m_max_ranges; ///< max. range list

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLTableObject();
    KVSMLTableObject( const std::string& filename );

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    const kvs::kvsml::ObjectTag& objectTag() const { return m_object_tag; }
    size_t nrows() const { return m_nrows; }
    size_t ncolumns() const { return m_ncolumns; }
    const std::vector<std::string>& labelList() const { return m_labels; }
    const std::vector<kvs::AnyValueArray>& columnList() const { return m_columns; }
    const std::vector<bool>& hasMinValueList() const { return m_has_min_values; }
    const std::vector<bool>& hasMaxValueList() const { return m_has_max_values; }
    const std::vector<bool>& hasMinRangeList() const { return m_has_min_ranges; }
    const std::vector<bool>& hasMaxRangeList() const { return m_has_max_ranges; }
    const std::vector<double>& minValueList() const { return m_min_values; }
    const std::vector<double>& maxValueList() const { return m_max_values; }
    const std::vector<double>& minRangeList() const { return m_min_ranges; }
    const std::vector<double>& maxRangeList() const { return m_max_ranges; }

    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setWritingDataTypeToAscii() { this->setWritingDataType( Ascii ); }
    void setWritingDataTypeToExternalAscii() { this->setWritingDataType( ExternalAscii ); }
    void setWritingDataTypeToExternalBinary() { this->setWritingDataType( ExternalBinary ); }
    void setMinValue( const size_t column_index, const double value );
    void setMaxValue( const size_t column_index, const double value );
    void setMinRange( const size_t column_index, const double range );
    void setMaxRange( const size_t column_index, const double range );
    void addColumn( const kvs::AnyValueArray& column, const std::string& label );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs
