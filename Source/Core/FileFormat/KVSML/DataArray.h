/*****************************************************************************/
/**
 *  @file   DataArray.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataArray.h 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/File>
#include <kvs/Tokenizer>
#include <kvs/ValueArray>
#include <kvs/AnyValueArray>
#include <kvs/IgnoreUnusedVariable>
#include <iostream>
#include <fstream>
#include <sstream>


namespace kvs
{

namespace kvsml
{

namespace temporal
{

template <typename T>
inline T To( const std::string& value )
{
    return static_cast<T>( atof( value.c_str() ) );
}

template <>
inline kvs::Int8 To( const std::string& value )
{
    return static_cast<kvs::Int8>( atoi( value.c_str() ) );
}

template <>
inline kvs::UInt8 To( const std::string& value )
{
    return static_cast<kvs::UInt8>( atoi( value.c_str() ) );
}

inline std::string TypeName( const std::type_info& type )
{
    if (      type == typeid( kvs::Int8   ) ) return "char";
    else if ( type == typeid( kvs::UInt8  ) ) return "uchar";
    else if ( type == typeid( kvs::Int16  ) ) return "short";
    else if ( type == typeid( kvs::UInt16 ) ) return "ushort";
    else if ( type == typeid( kvs::Int32  ) ) return "int";
    else if ( type == typeid( kvs::UInt32 ) ) return "uint";
    else if ( type == typeid( kvs::Int64  ) ) return "long";
    else if ( type == typeid( kvs::UInt64 ) ) return "ulong";
    else if ( type == typeid( kvs::Real32 ) ) return "float";
    else if ( type == typeid( kvs::Real64 ) ) return "double";
    else return "unknown";
}

/*===========================================================================*/
/**
 *  @brief  Reads the internal data as value array.
 *  @param  nelements  [in] number of elements
 *  @param  tokenizer  [in] tokenizer
 *  @return read data
 */
/*===========================================================================*/
template <typename T>
inline kvs::ValueArray<T> ReadInternalData(
    const size_t nelements,
    kvs::Tokenizer& tokenizer )
{
    kvs::ValueArray<T> result( nelements );
    for ( size_t i = 0; i < nelements; i++ )
    {
        result[i] = kvs::kvsml::temporal::To<T>( tokenizer.token() );
    }
    return result;
}

}

namespace DataArray
{

/*===========================================================================*/
/**
 *  @brief  Returns the data file name
 *  @param  filename [in] filename
 *  @param  type [in] data type (ex. 'value' or 'coord')
 *  @return data file name
 */
/*===========================================================================*/
inline std::string GetDataFilename( const std::string& filename, const std::string& type )
{
    const std::string basename( kvs::File( filename ).baseName() );
//    const std::string pathname( kvs::File( filename ).pathName() );
//    const std::string sep( kvs::File::Separator() );
    const std::string ext( "dat" );

//    return( pathname + sep + basename + "_" + type + "." + ext );
    return basename + "_" + type + "." + ext;
}

/*===========================================================================*/
/**
 *  @brief  Returns the data type of the given value array.
 *  @param  data_array [in] data array
 *  @return data type as string
 */
/*===========================================================================*/
template <typename T>
inline std::string GetDataType( const kvs::ValueArray<T>& /*data_array*/ )
{
    const std::type_info& type = typeid( T );
    return kvs::kvsml::temporal::TypeName( type );
}

/*===========================================================================*/
/**
 *  @brief  Returns the data type of the given any-value array.
 *  @param  data_array [in] data array
 *  @return data type as string
 */
/*===========================================================================*/
inline std::string GetDataType( const kvs::AnyValueArray& data_array )
{
    const std::type_info& type = data_array.typeInfo()->type();
    return kvs::kvsml::temporal::TypeName( type );
}

/*===========================================================================*/
/**
 *  @brief  Reads the internal data as any-value array.
 *  @param  data_array [out] pointer to the any-value array
 *  @param  nelements  [in] number of elements
 *  @param  tokenizer  [in] tokenizer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
template <typename T>
inline bool ReadInternalData(
    kvs::AnyValueArray* data_array,
    const size_t nelements,
    kvs::Tokenizer& tokenizer )
{
    *data_array = kvs::AnyValueArray( kvs::kvsml::temporal::ReadInternalData<T>( nelements, tokenizer ) );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads the internal data as value array.
 *  @param  data_array [out] pointer to the value array
 *  @param  nelements  [in] number of elements
 *  @param  tokenizer  [in] tokenizer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
template <typename T>
inline bool ReadInternalData(
    kvs::ValueArray<T>* data_array,
    const size_t nelements,
    kvs::Tokenizer& tokenizer )
{
    *data_array = kvs::kvsml::temporal::ReadInternalData<T>( nelements, tokenizer );
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads the external data as any-value array.
 *  @param  data_array [out] pointer to the any-value array
 *  @param  nelements  [in] number of elements
 *  @param  filename   [in] external file name
 *  @param  format     [in] file format (binary or ascii)
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
template <typename T>
inline bool ReadExternalData(
    kvs::AnyValueArray* data_array,
    const size_t nelements,
    const std::string& filename,
    const std::string& format )
{
    data_array->template allocate<T>( nelements );

    if ( format == "binary" )
    {
        FILE* ifs = fopen( filename.c_str(), "rb" );
        if( !ifs )
        {
            kvsMessageError("Cannot open '%s'.", filename.c_str());
            return false;
        }

        const size_t data_size = data_array->size();
        if ( fread( data_array->data(), sizeof(T), data_size, ifs ) != data_size )
        {
            kvsMessageError("Cannot read '%s'.", filename.c_str());
            fclose( ifs );
            return false;
        }

        fclose( ifs );
    }
    else if ( format == "ascii" )
    {
        FILE* ifs = fopen( filename.c_str(), "r" );
        if( !ifs )
        {
            kvsMessageError("Cannot open '%s'.", filename.c_str());
            return false;
        }

        fseek( ifs, 0, SEEK_END );
        const size_t size = ftell( ifs );

        char* buffer = static_cast<char*>( malloc( sizeof(char) * size ) );
        if ( !buffer )
        {
            kvsMessageError("Cannot allocate memory for reading the external data.");
            fclose( ifs );
            return false;
        }

        fseek( ifs, 0, SEEK_SET );
        if ( size != fread( buffer, 1, size, ifs ) )
        {
            kvsMessageError( "Cannot read '%s'.", filename.c_str() );
            return false;
        }

        T* data = static_cast<T*>( data_array->data() );

        const char* delim = " ,\t\n";
        char* value = strtok( buffer, delim );
        for ( size_t i = 0; i < nelements; i++ )
        {
            if ( value )
            {
                *(data++) = static_cast<T>( atof( value ) );
                value = strtok( 0, delim );
            }
        }

        free( buffer );

        fclose( ifs );
    }
    else
    {
        kvsMessageError("Unknown format '%s'.",format.c_str());
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads the external data as value array.
 *  @param  data_array [out] pointer to the value array
 *  @param  nelements  [in] number of elements
 *  @param  filename   [in] external file name
 *  @param  format     [in] file format (binary or ascii)
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
template <typename T1, typename T2>
inline bool ReadExternalData(
    kvs::ValueArray<T1>* out_array,
    const size_t nelements,
    const std::string& filename,
    const std::string& format )
{
    kvs::ValueArray<T1> data_array( nelements );

    if ( format == "binary" )
    {
        FILE* ifs = fopen( filename.c_str(), "rb" );
        if( !ifs )
        {
            kvsMessageError( "Cannot open '%s'.", filename.c_str() );
            return false;
        }

        if ( typeid( T1 ) == typeid( T2 ) )
        {
            const size_t data_size = data_array.size();
            if ( fread( data_array.data(), sizeof( T1 ), data_size, ifs ) != data_size )
            {
                kvsMessageError( "Cannot read '%s'.",filename.c_str() );
                fclose( ifs );
                return false;
            }
        }
        else
        {
            const size_t nloops = data_array.size();
            for ( size_t i = 0; i < nloops; i++ )
            {
                T2 data = T2(0);
                if ( fread( &data, sizeof(T2), 1, ifs ) != 1 )
                {
                    kvsMessageError( "Cannot read '%s'.",filename.c_str() );
                    fclose( ifs );
                    return false;
                }
                data_array[i] = static_cast<T1>( data );
            }
        }
        fclose( ifs );
    }
    else if ( format == "ascii" )
    {
        FILE* ifs = fopen( filename.c_str(), "r" );
        if ( !ifs )
        {
            kvsMessageError( "Cannot open '%s'.", filename.c_str() );
            return false;
        }

        fseek( ifs, 0, SEEK_END );
        const size_t size = ftell( ifs );

        std::vector<char> buffer( size );

        fseek( ifs, 0, SEEK_SET );
        if ( size != fread( &( buffer[0] ), 1, size, ifs ) )
        {
            kvsMessageError( "Cannot read '%s'.", filename.c_str() );
            return false;
        }

        T1* data = data_array.data();

        const char* delim = " ,\t\n";
        char* value = strtok( &( buffer[0] ), delim );
        for ( size_t i = 0; i < nelements; i++ )
        {
            if ( value )
            {
                *( data++ ) = static_cast<T1>( atof( value ) );
                value = strtok( 0, delim );
            }
        }

        fclose( ifs );
    }
    else
    {
        kvsMessageError( "Unknown format '%s'.",format.c_str() );
        return false;
    }

    *out_array = data_array;
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the external data as any-value array.
 *  @param  data_array [in] data array
 *  @param  filename [in] output file name
 *  @param  format [in] output file format
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
inline bool WriteExternalData(
    const kvs::AnyValueArray& data_array,
    const std::string& filename,
    const std::string& format )
{
    if ( format == "ascii" )
    {
        std::ofstream ofs( filename.c_str() );
        if ( ofs.fail() )
        {
            kvsMessageError("Cannot open file '%s'.", filename.c_str() );
            return false;
        }

        const std::string delim(", ");
        const std::type_info& data_type = data_array.typeInfo()->type();
        const size_t data_size = data_array.size();
        if ( data_type == typeid(kvs::Int8) )
        {
            const kvs::Int8* values = static_cast<const kvs::Int8*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << kvs::Int16(values[i]) << delim;
        }
        else if ( data_type == typeid(kvs::UInt8) )
        {
            const kvs::UInt8* values = static_cast<const kvs::UInt8*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << kvs::UInt16(values[i]) << delim;
        }
        else if ( data_type == typeid(kvs::Int16) )
        {
            const kvs::Int16* values = static_cast<const kvs::Int16*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }
        else if ( data_type == typeid(kvs::UInt16) )
        {
            const kvs::UInt16* values = static_cast<const kvs::UInt16*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }
        else if ( data_type == typeid(kvs::Int32) )
        {
            const kvs::Int32* values = static_cast<const kvs::Int32*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }
        else if ( data_type == typeid(kvs::UInt32) )
        {
            const kvs::UInt32* values = static_cast<const kvs::UInt32*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }
        else if ( data_type == typeid(kvs::Real32) )
        {
            const kvs::Real32* values = static_cast<const kvs::Real32*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }
        else if ( data_type == typeid(kvs::Real64) )
        {
            const kvs::Real64* values = static_cast<const kvs::Real64*>( data_array.data() );
            for ( size_t i = 0; i < data_size; i++ ) ofs << values[i] << delim;
        }

        ofs.close();
    }
    else if ( format == "binary" )
    {
        std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
        if ( ofs.fail() )
        {
            kvsMessageError("Cannot open file '%s'.", filename.c_str() );
            return false;
        }
        const void* data_pointer = data_array.data();
        const size_t data_byte_size = data_array.byteSize();
        ofs.write( static_cast<const char*>(data_pointer), data_byte_size );
        ofs.close();
    }
    else
    {
        kvsMessageError("Unknown format '%s'.",format.c_str());
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the external data as value array.
 *  @param  data_array [in] data array
 *  @param  filename [in] output file name
 *  @param  format [in] output file format
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
template <typename T>
inline bool WriteExternalData(
    const kvs::ValueArray<T>& data_array,
    const std::string& filename,
    const std::string& format )
{
    if ( format == "ascii" )
    {
        std::ofstream ofs( filename.c_str() );
        if ( ofs.fail() )
        {
            kvsMessageError("Cannot open file '%s'.", filename.c_str() );
            return false;
        }

        const std::string delim(", ");
        const size_t data_size = data_array.size();
        if ( typeid(T) == typeid(kvs::Int8) || typeid(T) == typeid(kvs::UInt8) )
        {
            for ( size_t i = 0; i < data_size; i++ ) ofs << int( data_array[i] ) << delim;
        }
        else
        {
            for ( size_t i = 0; i < data_size; i++ ) ofs << data_array[i] << delim;
        }

        ofs.close();
    }
    else if ( format == "binary" )
    {
        std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
        if ( ofs.fail() )
        {
            kvsMessageError("Cannot open file '%s'.", filename.c_str() );
            return false;
        }
        const char* data_pointer = reinterpret_cast<const char*>( data_array.data() );
        const size_t data_byte_size = data_array.byteSize();
        ofs.write( data_pointer, data_byte_size );
        ofs.close();
    }

    return true;
}

} // end of namespace DataArray

} // end of namespace kvsml

} // end of namespace kvs
