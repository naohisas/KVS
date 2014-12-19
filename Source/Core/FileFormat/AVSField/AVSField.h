/****************************************************************************/
/**
 *  @file AVSField.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AVSField.h 1511 2013-04-08 15:12:12Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__AVS_FIELD_H_INCLUDE
#define KVS__AVS_FIELD_H_INCLUDE

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <kvs/FileFormatBase>
#include <kvs/Vector3>
#include <kvs/AnyValueArray>
#include <kvs/Indent>


namespace kvs
{

/*==========================================================================*/
/**
 *  AVS field data class.
 */
/*==========================================================================*/
class AVSField : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

    enum FieldType
    {
        UnknownFieldType = 0, // unknown field type
        Uniform,              // uniform grid type
        Rectilinear,          // rectilinear grid type
        Irregular,            // irregular grid type
        NumberOfFieldTypes    // number of field data types
    };

    enum DataType
    {
        UnknownDataType = 0, // unknown data type
        Byte,                // byte (unsigned char) data type
        Short,               // short data type
        Integer,             // ineger data type
        Float,               // float data type
        Double,              // double data type
        NumberOfDataTypes    // number of data types
    };

private:

    int m_bits; ///< bits per voxel
    bool m_is_signed; ///< signed voxel [true/false]
    int m_veclen; ///< vector length
    int m_nspace; ///< number of spaces
    int m_ndim; ///< number of dimensions
    kvs::Vector3ui m_dim; ///< dimention
    kvs::Vector3f m_min_ext; ///< minimum external coordinate value
    kvs::Vector3f m_max_ext; ///< maximum external coordinate value
    bool m_has_min_max_ext; ///< has min max external coord
    FieldType m_field; ///< field data type
    DataType m_type; ///< data type
    std::vector<std::string> m_labels; ///< label
    kvs::AnyValueArray m_values; ///< field value array (shared array)
    kvs::ValueArray<float> m_coords; ///< coordinate value array (shared array)

public:

    static bool CheckExtension( const std::string& filename );

public:

    AVSField();
    AVSField( const std::string& filename );
    virtual ~AVSField();

    int bits() const;
    bool isSigned() const;
    int veclen() const;
    int nspace() const;
    int ndim() const;
    const kvs::Vector3ui& dim() const;
    const kvs::Vector3f& minExt() const;
    const kvs::Vector3f& maxExt() const;
    bool hasMinMaxExt() const;
    const std::string& field() const;
    const std::string& type() const;
    FieldType fieldType() const;
    DataType dataType() const;
    const std::vector<std::string>& labels() const;
    const kvs::AnyValueArray& values() const;
    const kvs::ValueArray<float>& coords() const;

    void setBits( const int bits );
    void setSigned( const bool sign );
    void setVeclen( const int veclen );
    void setNSpace( const int nspace );
    void setNDim( const int ndim );
    void setDim( const kvs::Vector3ui& dim );
    void setMinExt( const kvs::Vector3f& m );
    void setMaxExt( const kvs::Vector3f& m );
    void setFieldType( const FieldType field );
    void setDataType( const DataType type );
    void setLabels( const std::vector<std::string>& labels );
    void setValues( const kvs::AnyValueArray& values );
    void setCoords( const kvs::ValueArray<float>& coords );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    bool read_header( FILE* ifs );
    bool read_comment( FILE* ifs );
    bool read_node( FILE* ifs );
    bool read_coord( FILE* ifs );
    template< typename T >
    bool read_coord_data( FILE* ifs, const size_t nvertices );
    bool write_header( std::ofstream& ofs ) const;
    bool write_node( std::ofstream& ofs ) const;
};

} // end of namespace kvs

#endif // KVS__AVS_FIELD_H_INCLUDE
