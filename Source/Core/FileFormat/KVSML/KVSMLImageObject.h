/****************************************************************************/
/**
 *  @file   KVSMLImageObject.h
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
#include <kvs/Deprecated>
#include "KVSMLTag.h"
#include "ObjectTag.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KVSML image object format.
 */
/*===========================================================================*/
class KVSMLImageObject : public kvs::FileFormatBase
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
    size_t m_width; ///< image width
    size_t m_height; ///< image height
    std::string m_pixel_type; ///< pixel type
    WritingDataType m_writing_type; ///< writing data type
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< color(r,g,b) array

public:
    static bool CheckExtension( const std::string& filename );
    static bool CheckFormat( const std::string& filename );

public:
    KVSMLImageObject();
    KVSMLImageObject( const std::string& filename );

    const kvs::kvsml::KVSMLTag& KVSMLTag() const { return m_kvsml_tag; }
    const kvs::kvsml::ObjectTag& objectTag() const { return m_object_tag; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    const std::string& pixelType() const { return m_pixel_type; }
    const kvs::ValueArray<kvs::UInt8>& pixels() const { return m_pixels; }

    void setWidth( const size_t width ) { m_width = width; }
    void setHeight( const size_t height ) { m_height = height; }
    void setPixelType( const std::string& type ) { m_pixel_type = type; }
    void setWritingDataType( const WritingDataType type ) { m_writing_type = type; }
    void setPixels( const kvs::ValueArray<kvs::UInt8>& pixels ) { m_pixels = pixels; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
    KVS_DEPRECATED( void setData( const kvs::ValueArray<kvs::UInt8>& data ) ) { this->setPixels( data ); }
};

} // end of namespace kvs
