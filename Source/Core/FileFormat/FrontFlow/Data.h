/*****************************************************************************/
/**
 *  @file   Data.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/ValueArray>
#include <string>
#include <iostream>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  GF Data class.
 */
/*===========================================================================*/
class Data
{
private:
    std::string m_array_type_header = ""; ///< array type header
    std::string m_keyword = ""; ///< keyword
    std::string m_comment = ""; ///< comment (data name)
    kvs::Int32 m_num = 0; ///< num (vector length)
    kvs::Int32 m_num2 = 0; ///< num2 (number of elements)
    kvs::ValueArray<kvs::Real32> m_flt_array{}; ///< data array (float type)
    kvs::ValueArray<kvs::Int32> m_int_array{}; ///< data array (int type)

public:
    Data() = default;

    friend std::ostream& operator << ( std::ostream& os, const Data& d );

    const std::string& arrayTypeHeader() const { return m_array_type_header; }
    const std::string& keyword() const { return m_keyword; }
    const std::string& comment() const { return m_comment; }
    kvs::Int32 num() const { return m_num; }
    kvs::Int32 num2() const { return m_num2; }
    const kvs::ValueArray<kvs::Real32>& fltArray() const { return m_flt_array; }
    const kvs::ValueArray<kvs::Int32>& intArray() const { return m_int_array; }
    void deallocate();

    bool readAscii( FILE* fp, const std::string tag );
    bool readBinary( FILE* fp, const bool swap = false );
};

} // end of namespace gf

} // end of namespace kvs
