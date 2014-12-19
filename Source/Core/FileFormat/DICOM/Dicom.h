/****************************************************************************/
/**
 *  @file Dicom.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Dicom.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DICOM_H_INCLUDE
#define KVS__DICOM_H_INCLUDE

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <kvs/FileFormatBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector2>
#include <kvs/Indent>
#include "Attribute.h"
#include "Window.h"
#include "Element.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  DICOM class.
 */
/*===========================================================================*/
class Dicom : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef dcm::Element Element;
    typedef std::list<dcm::Element> ElementList;

protected:

    dcm::Attribute m_attribute; ///< attribute
    std::list<dcm::Element> m_element_list; ///< element list
    std::string m_modality; ///< modality (0008,0060)
    std::string m_manufacturer; ///< manufacturer (0008,0070)
    double m_slice_thickness; ///< slice thickness (0018,0050)
    double m_slice_spacing; ///< spacing between slices (0018,0088)
    int m_series_number; ///< series number (0020,0011)
    int m_image_number; ///< image number (0020,0013)
    double m_slice_location; ///< slice location (0020,1041)
    unsigned short m_row; ///< rows (height) (0028,0010)
    unsigned short m_column; ///< columns (width) (0028,0011)
    kvs::Vector2f m_pixel_spacing; ///< pixel spacing (0028,0030)
    unsigned short m_bits_allocated; ///< bits allocated (0028,0100)
    unsigned short m_bits_stored; ///< bits stored (0028,0101)
    unsigned short m_high_bit; ///< high bit (0028,0102)
    bool m_pixel_representation; ///< pixel representation (0028,0103)
    int m_window_level; ///< window center (0028,1050)
    int m_window_width; ///< window width (0028,1051)
    double m_rescale_intersept; ///< rescale intersept (0028,1052)
    double m_rescale_slope; ///< rescale slope (0028,1053)
    dcm::Window m_window; ///< window
    int m_min_raw_value; ///< min. value of the raw data
    int m_max_raw_value; ///< max. value of the raw data
    std::ios::pos_type m_position; ///< raw data position
    kvs::ValueArray<char> m_raw_data; ///< raw data

public:

    static bool CheckExtension( const std::string& filename );

public:

    Dicom();
    Dicom( const std::string& filename );
    virtual ~Dicom();

    unsigned short width() const;
    unsigned short height() const;
    int seriesNumber() const;
    int imageNumber() const;
    double sliceLocation() const;
    unsigned short row() const;
    unsigned short column() const;
    unsigned int size() const;
    unsigned short bitsAllocated() const;
    unsigned short bytesAllocated() const;
    unsigned short bitsStored() const;
    unsigned short bytesStored() const;
    unsigned short highBit() const;
    bool pixelRepresentation() const;
    int windowLevel() const;
    int windowWidth() const;
    int minWindowValue() const;
    int maxWindowValue() const;
    int minRawValue() const;
    int maxRawValue() const;
    int minValue() const;
    int maxValue() const;
    double rescaleIntersept() const;
    double rescaleSlope() const;
    const dcm::Attribute& attribute() const;
    const std::list<dcm::Element>& elementList() const;
    const std::string& modality() const;
    const kvs::Vector2f& pixelSpacing() const;
    const std::string& manufacturer() const;
    double sliceThickness() const;
    double sliceSpacing() const;
    const dcm::Window& window() const;
    const std::ios::pos_type& position() const;
    const kvs::ValueArray<char>& rawData() const;
    kvs::ValueArray<kvs::UInt8> pixelData() const;
    int rawValue( const size_t index ) const;
    int rawValue( const size_t i, const size_t j ) const;
    int value( const size_t index ) const;
    int value( const size_t x, const size_t y ) const;

    void setRawData( const kvs::ValueArray<char>& raw_data );
    void changeWindow( const int level, const int width );
    void resetWindow();
    std::list<dcm::Element>::iterator findElement( const dcm::Tag tag );
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    bool read_header( std::ifstream& ifs );
    bool read_data( std::ifstream& ifs );
    bool write_header( std::ofstream& ofs );
    bool write_header_csv( std::ofstream& ofs );
    bool write_raw_data( std::ofstream& ofs );
    void set_windowing_parameter();
    void set_min_max_window_value();
    template <typename T>
    void calculate_min_max_raw_value();
    void set_min_max_raw_value();
    template <typename T>
    kvs::ValueArray<kvs::UInt8> rescale_pixel_data( const int level, const int width ) const;
    kvs::ValueArray<kvs::UInt8> get_pixel_data( const int level, const int width ) const;
    void parse_element( dcm::Element& element );
};

} // end of namespace kvs

#endif // KVS__DICOM_H_INCLUDE
