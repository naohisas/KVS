/****************************************************************************/
/**
 *  @file   AVSField.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    using BaseClass = kvs::FileFormatBase;

    enum FieldType
    {
        UnknownFieldType = 0, // unknown field type
        Uniform, // uniform grid type
        Rectilinear, // rectilinear grid type
        Irregular, // irregular grid type
        NumberOfFieldTypes // number of field data types
    };

    enum DataType
    {
        UnknownDataType = 0, // unknown data type
        Byte, // byte (unsigned char) data type
        Short, // short data type
        Integer, // ineger data type
        Float, // float data type
        Double, // double data type
        NumberOfDataTypes // number of data types
    };

private:
    int m_bits = 0; ///< bits per voxel
    bool m_is_signed = true; ///< signed voxel [true/false]
    int m_veclen = 1; ///< vector length
    int m_nspace = 0; ///< number of spaces
    int m_ndim = 0; ///< number of dimensions
    kvs::Vec3ui m_dim{ 1, 1, 1 }; ///< dimention
    kvs::Vec3 m_min_ext{ 0, 0, 0 }; ///< minimum external coordinate value
    kvs::Vec3 m_max_ext{ 1, 1, 1 }; ///< maximum external coordinate value
    bool m_has_min_max_ext = false; ///< has min max external coord
    FieldType m_field = FieldType::Uniform; ///< field data type
    DataType m_type = DataType::UnknownDataType; ///< data type
    std::vector<std::string> m_labels{}; ///< label
    kvs::AnyValueArray m_values{}; ///< field value array (shared array)
    kvs::ValueArray<float> m_coords{}; ///< coordinate value array (shared array)

public:
    static bool CheckExtension( const std::string& filename );

public:
    AVSField() = default;
    virtual ~AVSField() = default;
    AVSField( const std::string& filename ) { this->read( filename ); }

    int bits() const { return m_bits; }
    bool isSigned() const { return m_is_signed; }
    int veclen() const { return m_veclen; }
    int nspace() const { return m_nspace; }
    int ndim() const { return m_ndim; }
    const kvs::Vec3ui& dim() const { return m_dim; }
    const kvs::Vec3& minExt() const { return m_min_ext; }
    const kvs::Vec3& maxExt() const { return m_max_ext; }
    bool hasMinMaxExt() const { return m_has_min_max_ext; }
    const std::string& field() const;
    const std::string& type() const;
    FieldType fieldType() const { return m_field; }
    DataType dataType() const { return m_type; }
    const std::vector<std::string>& labels() const { return m_labels; }
    const kvs::AnyValueArray& values() const { return m_values; }
    const kvs::ValueArray<float>& coords() const { return m_coords; }

    void setBits( const int bits ) { m_bits = bits; }
    void setSigned( const bool sign ) { m_is_signed = sign; }
    void setVeclen( const int veclen ) { m_veclen = veclen; }
    void setNSpace( const int nspace ) { m_nspace = nspace; }
    void setNDim( const int ndim ) { m_ndim = ndim; }
    void setDim( const kvs::Vec3ui& dim ) { m_dim = dim; }
    void setMinExt( const kvs::Vec3& m ) { m_has_min_max_ext = true; m_min_ext = m; }
    void setMaxExt( const kvs::Vec3& m ) { m_has_min_max_ext = true; m_max_ext = m; }
    void setFieldType( const FieldType field ) { m_field = field; }
    void setDataType( const DataType type ) { m_type = type; }
    void setLabels( const std::vector<std::string>& labels );
    void setValues( const kvs::AnyValueArray& values ) { m_values = values; }
    void setCoords( const kvs::ValueArray<float>& coords ) { m_coords = coords; }

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
