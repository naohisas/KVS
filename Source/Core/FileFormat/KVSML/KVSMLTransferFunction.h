/****************************************************************************/
/**
 *  @file   KVSMLTransferFunction.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KVSMLTransferFunction.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <list>
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/RGBColor>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>
#include "KVSMLTag.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSMLTransferFunction file format class.
 */
/*===========================================================================*/
class KVSMLTransferFunction : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;
    typedef std::pair<float,kvs::Real32> OpacityPoint;
    typedef std::list<OpacityPoint> OpacityPointList;
    typedef std::pair<float,kvs::RGBColor> ColorPoint;
    typedef std::list<ColorPoint> ColorPointList;

    enum WritingDataType
    {
        Ascii = 0,
        ExternalAscii,
        ExternalBinary
    };

private:
    kvs::kvsml::KVSMLTag m_kvsml_tag; ///< KVSML tag information
    WritingDataType m_writing_type; ///< writing data type
    size_t m_resolution; ///< resolution
    float m_min_value; ///< min. scalar value
    float m_max_value; ///< max. scalar value
    OpacityPointList m_opacity_point_list; ///< opacity point list
    ColorPointList m_color_point_list; ///< color point list
    kvs::ValueArray<kvs::Real32> m_opacities; ///< opacity array
    kvs::ValueArray<kvs::UInt8> m_colors; ///< color (r,g,b) array

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLTransferFunction();
    KVSMLTransferFunction( const std::string& filename );
    virtual ~KVSMLTransferFunction();

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    size_t resolution() const { return m_resolution; }
    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }
    const OpacityPointList& opacityPointList() const { return m_opacity_point_list; }
    const ColorPointList& colorPointList() const { return m_color_point_list; }
    const kvs::ValueArray<kvs::Real32>& opacities() const { return m_opacities; }
    const kvs::ValueArray<kvs::UInt8>& colors() const { return m_colors; }

    void setResolution( const size_t resolution ) { m_resolution = resolution; }
    void setRange( const float min_value, const float max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setWritingDataTypeToAscii() { this->setWritingDataType( Ascii ); }
    void setWritingDataTypeToExternalAscii() { this->setWritingDataType( ExternalAscii ); }
    void setWritingDataTypeToExternalBinary() { this->setWritingDataType( ExternalBinary ); }
    void setOpacities( const kvs::ValueArray<kvs::Real32>& opacities ) { m_opacities = opacities; }
    void setColors( const kvs::ValueArray<kvs::UInt8>& colors ) { m_colors = colors; }
    void addOpacityPoint( const float value, const float opacity );
    void addColorPoint( const float value, const kvs::RGBColor color );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );
};

} // end of namespace kvs
