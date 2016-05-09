/****************************************************************************/
/**
 *  @file Dicom.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Dicom.cpp 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <climits>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <kvs/File>
#include <kvs/Math>
#include <kvs/Message>
#include <kvs/Tokenizer>
#include <kvs/Type>
#include <kvs/File>
#include <kvs/Value>
#include <kvs/ValueArray>
#include <kvs/IgnoreUnusedVariable>
#include "Dicom.h"
#include "VRType.h"
#include "Element.h"
#include "Value.h"


namespace
{
kvs::dcm::Tag END_HEADER_TAG = kvs::dcm::Tag( 0x7FE0, 0x0010, kvs::dcm::VR_OW );
}

namespace kvs
{

bool Dicom::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "dcm"   || file.extension() == "DCM" ||
         file.extension() == "dicom" || file.extension() == "DICOM" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Default constructor.
 */
/*===========================================================================*/
Dicom::Dicom():
    m_modality( "unknown" ),
    m_manufacturer( "unknown" ),
    m_slice_thickness( 0.0 ),
    m_slice_spacing( 0.0 ),
    m_series_number( 0 ),
    m_image_number( 0 ),
    m_slice_location( 0.0 ),
    m_row( 0 ),
    m_column( 0 ),
    m_pixel_spacing( 0.0f, 0.0f ),
    m_bits_allocated( 0 ),
    m_bits_stored( 0 ),
    m_high_bit( 0 ),
    m_pixel_representation( true ),
    m_window_level( 0 ),
    m_window_width( 0 ),
    m_rescale_intersept( 0.0 ),
    m_rescale_slope( 1.0 ),
    m_min_raw_value( 0 ),
    m_max_raw_value( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Construct a new class.
 *  @param  filename [i] filename
 */
/*===========================================================================*/
Dicom::Dicom( const std::string& filename ):
    m_modality( "unknown" ),
    m_manufacturer( "unknown" ),
    m_slice_thickness( 0.0 ),
    m_slice_spacing( 0.0 ),
    m_series_number( 0 ),
    m_image_number( 0 ),
    m_slice_location( 0.0 ),
    m_row( 0 ),
    m_column( 0 ),
    m_pixel_spacing( 0.0f, 0.0f ),
    m_bits_allocated( 0 ),
    m_bits_stored( 0 ),
    m_high_bit( 0 ),
    m_pixel_representation( true ),
    m_window_level( 0 ),
    m_window_width( 0 ),
    m_rescale_intersept( 0.0 ),
    m_rescale_slope( 1.0 ),
    m_min_raw_value( 0 ),
    m_max_raw_value( 0 )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Destructor.
 */
/*===========================================================================*/
Dicom::~Dicom()
{
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::width() const
{
    return m_column;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::height() const
{
    return m_row;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::seriesNumber() const
{
    return m_series_number;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::imageNumber() const
{
    return m_image_number;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
double Dicom::sliceLocation() const
{
    return m_slice_location;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::row() const
{
    return m_row;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::column() const
{
    return m_column;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned int Dicom::size() const
{
    return m_row * m_column * ( m_bits_allocated >> 3 );
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::bitsAllocated() const
{
    return m_bits_allocated;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::bytesAllocated() const
{
    return m_bits_allocated >> 3;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::bitsStored() const
{
    return m_bits_stored;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::bytesStored() const
{
    return m_bits_stored >> 3;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
unsigned short Dicom::highBit() const
{
    return m_high_bit;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
bool Dicom::pixelRepresentation() const
{
    return m_pixel_representation;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::windowLevel() const
{
    return m_window_level;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::windowWidth() const
{
    return m_window_width;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::minWindowValue() const
{
    return m_window.minValue();
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::maxWindowValue() const
{
    return m_window.maxValue();
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::minRawValue() const
{
    return m_min_raw_value;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::maxRawValue() const
{
    return m_max_raw_value;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::minValue() const
{
    return kvs::Math::Round( m_min_raw_value * m_rescale_slope + m_rescale_intersept );
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
int Dicom::maxValue() const
{
    return kvs::Math::Round( m_max_raw_value * m_rescale_slope + m_rescale_intersept );
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
double Dicom::rescaleIntersept() const
{
    return m_rescale_intersept;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
double Dicom::rescaleSlope() const
{
    return m_rescale_slope;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
const dcm::Attribute& Dicom::attribute() const
{
    return m_attribute;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
const std::list<dcm::Element>& Dicom::elementList() const
{
    return m_element_list;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
const std::string& Dicom::modality() const
{
    return m_modality;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
const kvs::Vector2f& Dicom::pixelSpacing() const
{
    return m_pixel_spacing;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
const std::string& Dicom::manufacturer() const
{
    return m_manufacturer;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
double Dicom::sliceThickness() const
{
    return m_slice_thickness;
}

/*===========================================================================*/
/**
 *  @brief  
 *  @return  
 */
/*===========================================================================*/
double Dicom::sliceSpacing() const
{
    return m_slice_spacing;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
const dcm::Window& Dicom::window() const
{
    return m_window;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
const std::ios::pos_type& Dicom::position() const
{
    return m_position;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
const kvs::ValueArray<char>& Dicom::rawData() const
{
    return m_raw_data;
}

/*===========================================================================*/
/**
 *  @brief  
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> Dicom::pixelData() const
{
    return this->get_pixel_data( m_window.level(), m_window.width() );
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  index
 *  @return  
 */
/*===========================================================================*/
int Dicom::rawValue( const size_t index ) const
{
    int ret = 0;

    if( m_bits_allocated == 8 )
    {
        if( m_pixel_representation )
        {
            const kvs::UInt8* raw_data = reinterpret_cast<const kvs::UInt8*>( m_raw_data.data() );
            ret = raw_data[index];
        }
        else
        {
            const kvs::Int8* raw_data = reinterpret_cast<const kvs::Int8*>( m_raw_data.data() );
            ret = raw_data[index];
        }
    }
    else if( m_bits_allocated == 16 )
    {
        if( m_pixel_representation )
        {
            const kvs::UInt16* raw_data = reinterpret_cast<const kvs::UInt16*>( m_raw_data.data() );
            ret = raw_data[index];
        }
        else
        {
            const kvs::Int16* raw_data = reinterpret_cast<const kvs::Int16*>( m_raw_data.data() );
            ret = raw_data[index];
        }
    }

    return ret;
}

/*===========================================================================*/
/**
 *  @brief  
 *
 *  @param  i   
 *  @param  j   
 *  @return 
 */
/*===========================================================================*/
int Dicom::rawValue( const size_t i, const size_t j ) const
{
    return this->rawValue( m_column * j + i );
}

/*===========================================================================*/
/**
 *  @brief  Return a rescaled pixel value.
 *  @param  index [in] pixel index
 *  @return rescaled pixel value
 */
/*===========================================================================*/
int Dicom::value( const size_t index ) const
{
    const int raw_value = this->rawValue( index );

    return kvs::Math::Round( raw_value * m_rescale_slope + m_rescale_intersept );
}

/*===========================================================================*/
/**
 *  @brief  Return a rescaled pixel value.
 *  @param  x [in] index x
 *  @param  y [in] index y
 *  @return rescaled pixel value
 */
/*===========================================================================*/
int Dicom::value( const size_t x, const size_t y ) const
{
    const size_t raw_value = this->rawValue( x, y );

    return kvs::Math::Round( raw_value * m_rescale_slope + m_rescale_intersept );
}

/*===========================================================================*/
/**
 *  @brief  Change the windowing parameters.
 *  @param  level [in] window level
 *  @param  width [in] window width
 */
/*===========================================================================*/
void Dicom::changeWindow( const int level, const int width )
{
    // Set windowing parameters.
    m_window.setLevel( level );
    m_window.setWidth( width );
}

void Dicom::resetWindow()
{
    // Set windowing parameters.
    m_window.setLevel( m_window_level );
    m_window.setWidth( m_window_width );
}

/*===========================================================================*/
/**
 *  @brief  Set raw data.
 *  @param  raw_data [in] raw data
 */
/*===========================================================================*/
void Dicom::setRawData( const kvs::ValueArray<char>& raw_data )
{
    m_raw_data = raw_data;

    this->set_min_max_window_value();
    this->set_min_max_raw_value();
}

/*===========================================================================*/
/**
 *  @brief  Find the DICOM header element.
 *  @param  tag [in] element tag
 *  @return iterator to the element, if the given element is found
 */
/*===========================================================================*/
std::list<dcm::Element>::iterator Dicom::findElement( const dcm::Tag tag )
{
    return std::find( m_element_list.begin(), m_element_list.end(), tag );
}

void Dicom::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Modality : " <<  m_modality << std::endl;
    os << indent << "Manufacturer : " <<  m_manufacturer << std::endl;
    os << indent << "Slice thickness : " <<  m_slice_thickness << std::endl;
    os << indent << "Slice spacing : " <<  m_slice_spacing << std::endl;
    os << indent << "Series number : " <<  m_series_number << std::endl;
    os << indent << "Image number : " <<  m_image_number << std::endl;
    os << indent << "Slice location : " <<  m_slice_location << std::endl;
    os << indent << "Row : " <<  m_row << std::endl;
    os << indent << "Column : " <<  m_column << std::endl;
    os << indent << "Pixel spaceing : " <<  m_pixel_spacing[0] << " " << m_pixel_spacing[1] << std::endl;
    os << indent << "Bits allocated : " <<  m_bits_allocated << std::endl;
    os << indent << "Bits stored : " <<  m_bits_stored << std::endl;
    os << indent << "High bit : " <<  m_high_bit << std::endl;
    os << indent << "Pixel representation : " <<( m_pixel_representation ? "unsigned" : "signed" ) << std::endl;
    os << indent << "Window center : " <<  m_window_level << std::endl;
    os << indent << "Window width : " <<  m_window_width << std::endl;
    os << indent << "Rescale intersept : " <<  m_rescale_intersept << std::endl;
    os << indent << "Rescale slope : " <<  m_rescale_slope << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read the given file as DICOM format.
 *  @param  filename [i] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Dicom::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Open the file.
    std::ifstream ifs( filename.c_str(), std::ios_base::binary );
    if( ifs.fail() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    // Check attribute.
    if( !m_attribute.check( ifs ) )
    {
        kvsMessageError("Fail the attribute check of the DICOM file.");
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Read the header information.
    if( !this->read_header( ifs ) )
    {
        kvsMessageError("Cannot read the header of the DICOM file.");
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Read the pixel data.
    if( !this->read_data( ifs ) )
    {
        kvsMessageError("Cannot read the pixel data of the DICOM file.");
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write a file.
 *  @param  filename [in] output filename
 *  @return true if writing success, false if not.
 */
/*===========================================================================*/
bool Dicom::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
    if( ofs.fail() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    const kvs::File   file( filename );
    const std::string extension = file.extension();

    bool state = false;
    if(      extension == "hdr" ) state = this->write_header( ofs );
    else if( extension == "csv" ) state = this->write_header_csv( ofs );
    else if( extension == "raw" ) state = this->write_raw_data( ofs );

    if( !state )
    {
        kvsMessageError("Cannot write data.");
        BaseClass::setSuccess( false );
        return false;
    }

    ofs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read the header information of the DICOM file.
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Dicom::read_header( std::ifstream& ifs )
{
    // Read all elements.
    dcm::Element element;

    for( ; ; )
    {
        if( !element.read( ifs, m_attribute.swap() ) )
        {
            kvsMessageError("Cannot read the data element.");
            return false;
        }

#if   DCM_DEBUG__STDOUT_KNOWN_ELEMENTS
        if( element.isKnown() ) cout << element << std::endl << std::endl;
#elif DCM_DEBUG__STDOUT_ALL_ELEMENTS
        cout << element << std::endl << std::endl;
#endif
        if( element.isKnown() ) m_element_list.push_back( element );

        this->parse_element( element );

        if( element.tag() == ::END_HEADER_TAG )
        {
            m_position = ifs.tellg();
            break;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read the raw data and the pixel data.
 *  @param  ifs [in] input file stream
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Dicom::read_data( std::ifstream& ifs )
{
    // Go to the top of the pixel data region.
    ifs.seekg( m_position, std::ios::beg );

    // Read the raw data.
    const size_t raw_data_size = m_row * m_column * ( m_bits_allocated >> 3 );
    m_raw_data.allocate( raw_data_size );

    ifs.read( m_raw_data.data(), raw_data_size );
    if( ifs.bad() )
    {
        kvsMessageError("Cannot read the raw data.");
        return false;
    }

    this->set_min_max_window_value();
    this->set_min_max_raw_value();
    this->set_windowing_parameter();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the DICOM header information.
 *  @param  ofs [out] output file stream
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Dicom::write_header( std::ofstream& ofs )
{
    std::list<dcm::Element>::iterator p = m_element_list.begin();
    while( p != m_element_list.end() ) ofs << *p++ << std::endl << std::endl;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the DICOM header information as CSV format.
 *  @param  ofs [out] ouput file stream
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Dicom::write_header_csv( std::ofstream& ofs )
{
    ofs << "Group ID,"
        << "Element ID,"
        << "Element name,"
        << "VR type,"
        << "VR description,"
        << "Value length,"
        << "Value" << std::endl;

    std::list<dcm::Element>::iterator p = m_element_list.begin();
    while( p != m_element_list.end() )
    {
        const dcm::Element& element = *p;
        const dcm::Tag&     tag     = element.tag();
        const dcm::VR&      vr      = element.vr();
        const dcm::Value&   value   = element.value();
        ofs << "0x" << std::hex << std::setw(4) << std::setfill('0') << tag.groupID()   << ","
            << "0x" << std::hex << std::setw(4) << std::setfill('0') << tag.elementID() << ","
            << std::dec
            << tag.name() << ","
            << vr.vrTypeString() << ","
            << vr.vrTypeDescription() << ","
            << vr.valueLength() << ","
            << value << std::endl;

        p++;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the DICOM raw data.
 *  @param  ofs [out] ouput file stream
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Dicom::write_raw_data( std::ofstream& ofs )
{
    ofs.write( m_raw_data.data(), m_raw_data.size() );

    return true;
}

void Dicom::set_windowing_parameter()
{
    if( m_window_level == 0 && m_window_width == 0 )
    {
        int min_value = m_min_raw_value;
        int max_value = m_max_raw_value;
        if( m_bits_allocated == 8 )
        {
            if( m_pixel_representation )
            {
                min_value = kvs::Value<kvs::UInt8>::Min();
                max_value = kvs::Value<kvs::UInt8>::Max();
            }
            else
            {
                min_value = kvs::Value<kvs::Int8>::Min();
                max_value = kvs::Value<kvs::Int8>::Max();
            }
        }

        m_window_level = static_cast<int>( ( max_value + min_value ) / 2.0 );
        m_window_width = max_value - min_value + 1;
    }

    m_window.setLevel( m_window_level );
    m_window.setWidth( m_window_width );
}

/*===========================================================================*/
/**
 *  @brief  Set min/max window values.
 */
/*===========================================================================*/
void Dicom::set_min_max_window_value()
{
    m_window.set( m_bits_stored, !m_pixel_representation );
    m_window.rescale( m_rescale_slope, m_rescale_intersept );
}

/*===========================================================================*/
/**
 *  @brief  Calculate min/max raw value.
 */
/*===========================================================================*/
template <typename T>
void Dicom::calculate_min_max_raw_value()
{
    const T* raw_data = reinterpret_cast<const T*>( m_raw_data.data() );

    double min_raw_value = raw_data[0];
    double max_raw_value = raw_data[0];

    int npixels = m_column * m_row;
    for( int index = 1; index < npixels; index++ )
    {
        double raw_value  = raw_data[index];
        min_raw_value = kvs::Math::Min( min_raw_value, raw_value );
        max_raw_value = kvs::Math::Max( max_raw_value, raw_value );
    }

    m_min_raw_value = kvs::Math::Round( min_raw_value );
    m_max_raw_value = kvs::Math::Round( max_raw_value );
}

template void Dicom::calculate_min_max_raw_value<kvs::Int8>();
template void Dicom::calculate_min_max_raw_value<kvs::UInt8>();
template void Dicom::calculate_min_max_raw_value<kvs::Int16>();
template void Dicom::calculate_min_max_raw_value<kvs::UInt16>();

/*===========================================================================*/
/**
 *  @brief  Set min/max raw value.
 */
/*===========================================================================*/
void Dicom::set_min_max_raw_value()
{
    if( m_bits_allocated == 8 )
    {
        if( m_pixel_representation )
        {
            this->calculate_min_max_raw_value<kvs::UInt8>();
        }
        else
        {
            this->calculate_min_max_raw_value<kvs::Int8>();
        }
    }
    else if( m_bits_allocated == 16 )
    {
        if( m_pixel_representation )
        {
            this->calculate_min_max_raw_value<kvs::UInt16>();
        }
        else
        {
            this->calculate_min_max_raw_value<kvs::Int16>();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Rescale pixel data.
 */
/*===========================================================================*/
template <typename T>
kvs::ValueArray<kvs::UInt8> Dicom::rescale_pixel_data( const int level, const int width ) const
{
    kvs::ValueArray<kvs::UInt8> pixel_data( m_row * m_column );
    pixel_data.fill( 0 );

    const T* raw_data = reinterpret_cast<const T*>( m_raw_data.data() );

    const size_t npixels = m_row * m_column;
    for( size_t index = 0; index < npixels; index++ )
    {
        double temp_value  = raw_data[index] * m_rescale_slope + m_rescale_intersept;
        double pixel_value = ( ( temp_value - level ) / width + 0.5 ) * 255.0;
        pixel_value = kvs::Math::Clamp( pixel_value, 0.0, 255.0 );
        pixel_data[index] = static_cast<kvs::UInt8>( pixel_value );
    }

    return pixel_data;
}

// Specialization for 'kvs::Int8' type.
template <>
kvs::ValueArray<kvs::UInt8> Dicom::rescale_pixel_data<kvs::Int8>( const int level, const int width ) const
{
    kvs::IgnoreUnusedVariable( level );
    kvs::IgnoreUnusedVariable( width );

    kvs::ValueArray<kvs::UInt8> pixel_data( m_row * m_column );
    pixel_data.fill( 0 );

    const kvs::Int8* raw_data = reinterpret_cast<const kvs::Int8*>( m_raw_data.data() );

    const size_t npixels = m_row * m_column;
    for( size_t index = 0; index < npixels; index++ )
    {
        pixel_data[index] = static_cast<kvs::UInt8>( raw_data[index] - kvs::Value<kvs::Int8>::Min() );
    }

    return pixel_data;
}

// Specialization for 'kvs::UInt8' type.
template <>
kvs::ValueArray<kvs::UInt8> Dicom::rescale_pixel_data<kvs::UInt8>( const int level, const int width ) const
{
    kvs::IgnoreUnusedVariable( level );
    kvs::IgnoreUnusedVariable( width );

    kvs::ValueArray<kvs::UInt8> pixel_data( m_row * m_column );
    pixel_data.fill( 0 );

    const kvs::UInt8* raw_data = reinterpret_cast<const kvs::UInt8*>( m_raw_data.data() );

    const size_t npixels = m_row * m_column;
    for( size_t index = 0; index < npixels; index++ )
    {
        pixel_data[index] = raw_data[index];
    }

    return pixel_data;
}

template
kvs::ValueArray<kvs::UInt8> Dicom::rescale_pixel_data<kvs::Int16>( const int level, const int width ) const;

template
kvs::ValueArray<kvs::UInt8> Dicom::rescale_pixel_data<kvs::UInt16>( const int level, const int width ) const;

/*===========================================================================*/
/**
 *  @brief  Return pixel data array by executing the windowing process.
 *  @return pixel data array
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> Dicom::get_pixel_data( const int level, const int width ) const
{
    if( m_bits_allocated == 8 )
    {
        if( m_pixel_representation )
        {
            return this->rescale_pixel_data<kvs::UInt8>( level, width );
        }
        else
        {
            return this->rescale_pixel_data<kvs::Int8>( level, width );
        }
    }

    if( m_bits_allocated == 16 )
    {
        if( m_pixel_representation )
        {
            return this->rescale_pixel_data<kvs::UInt16>( level, width );
        }
        else
        {
            return this->rescale_pixel_data<kvs::Int16>( level, width );
        }
    }

    kvsMessageError("Cannot read the pixel data from the raw data.");

    kvs::ValueArray<kvs::UInt8> pixel_data( m_row * m_column );
    pixel_data.fill( 0 );

    return pixel_data;
}

/*===========================================================================*/
/**
 *  @brief  Parse the available element information.
 *  @param  element [i] DICOM element
 */
/*===========================================================================*/
void Dicom::parse_element( dcm::Element& element )
{
    // Modality.
    if( element.tag() == dcm::Tag( 0x0008, 0x0060 ) )
    {
        m_modality = (std::string)element.value();
        return;
    }

    // Manufacturer.
    if( element.tag() == dcm::Tag( 0x0008, 0x0070 ) )
    {
        m_manufacturer = (std::string)element.value();
        return;
    }

    // Slice thickness.
    if( element.tag() == dcm::Tag( 0x0018, 0x0050 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_slice_thickness;
        return;
    }

    // Slice spacing.
    if( element.tag() == dcm::Tag( 0x0018, 0x0088 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_slice_spacing;
        return;
    }

    // Series number.
    if( element.tag() == dcm::Tag( 0x0020, 0x0011 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_series_number;
        return;
    }

    // Image number.
    if( element.tag() == dcm::Tag( 0x0020, 0x0013 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_image_number;
        return;
    }

    // Slice location.
    if( element.tag() == dcm::Tag( 0x0020, 0x1041 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_slice_location;
        return;
    }

    // Column.
    if( element.tag() == dcm::Tag( 0x0028, 0x0011 ) )
    {
        m_column = element.value();
        return;
    }

    // Row.
    if( element.tag() == dcm::Tag( 0x0028, 0x0010 ) )
    {
        m_row = element.value();
        return;
    }

    // Bits allocated.
    if( element.tag() == dcm::Tag( 0x0028, 0x0100 ) )
    {
        m_bits_allocated = element.value();
        return;
    }

    // Bits stored.
    if( element.tag() == dcm::Tag( 0x0028, 0x0101 ) )
    {
        m_bits_stored = element.value();
        return;
    }

    // High bit.
    if( element.tag() == dcm::Tag( 0x0028, 0x0102 ) )
    {
        m_high_bit = element.value();
        return;
    }

    // Pixel representation.
    if( element.tag() == dcm::Tag( 0x0028, 0x0103 ) )
    {
        // (true: unsigned, false: signed)
        const unsigned short temp = element.value();
        m_pixel_representation = ( temp == 0 ) ? true : false;
        return;
    }

    // Pixel spacing.
    if( element.tag() == dcm::Tag( 0x0028, 0x0030 ) )
    {
        // Split the string by the delimiter '\': "xxx\yyy" -> xxx yyy
        std::string temp  = element.value();
        if ( temp.size() == 0 ) return;

        std::string delim = "\\";
        std::vector<std::string> tokens;

        kvs::Tokenizer t( temp, delim );
        while( !t.isLast() )
        {
            tokens.push_back( t.token() );
        }

        if( tokens.size() != 2 )
        {
            kvsMessageError("Wrong string for the \"pixel spacing\" tag. (%s)",temp.c_str());
            return;
        }

        std::stringstream t0( tokens[0] ); t0 >> m_pixel_spacing[0];
        std::stringstream t1( tokens[1] ); t1 >> m_pixel_spacing[1];

        return;
    }

    // Window center.
    if( element.tag() == dcm::Tag( 0x0028, 0x1050 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_window_level;
        return;
    }

    // Window width.
    if( element.tag() == dcm::Tag( 0x0028, 0x1051 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_window_width;
        return;
    }

    // Rescale intersept.
    if( element.tag() == dcm::Tag( 0x0028, 0x1052 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_rescale_intersept;
        return;
    }

    // Rescale slope.
    if( element.tag() == dcm::Tag( 0x0028, 0x1053 ) )
    {
        std::string temp = element.value();
        if ( temp.size() == 0 ) return;

        std::stringstream t( temp );
        t >> m_rescale_slope;
        return;
    }
}

} // end of namespace kvs
