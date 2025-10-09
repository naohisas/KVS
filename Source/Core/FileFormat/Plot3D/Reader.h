/*****************************************************************************/
/**
 *  @file   Reader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <fstream>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/ValueArray>


namespace kvs
{

namespace plot3d
{

namespace Reader
{

template <typename T>
inline T Peek( std::ifstream& ifs )
{
    T ret{ 0 };
    {
        auto p = ifs.tellg();
        ifs.read( (char*)&ret, sizeof(T) );
        ifs.seekg( p );
    }
    return ret;
}

inline void Seek( std::ifstream& ifs, const size_t bytes )
{
    ifs.seekg( bytes, std::ios_base::cur );
}

template <typename T>
inline T Value( std::ifstream& ifs, const size_t offset = 4 )
{
    T ret{ 0 };
    {
        ifs.seekg( offset, std::ios_base::cur );
        ifs.read( (char*)&ret, sizeof(T) );
        ifs.seekg( offset, std::ios_base::cur );
    }
    return ret;
}

template <typename T>
inline kvs::Vector3<T> Vec3( std::ifstream& ifs, const size_t offset )
{
    kvs::Vector3<T> ret{ 0, 0, 0 };
    {
        ifs.seekg( offset, std::ios_base::cur );
        ifs.read( (char*)&ret.x(), sizeof(T) );
        ifs.read( (char*)&ret.y(), sizeof(T) );
        ifs.read( (char*)&ret.z(), sizeof(T) );
        ifs.seekg( offset, std::ios_base::cur );
    }
    return ret;
}

template <typename T>
inline kvs::Vector4<T> Vec4( std::ifstream& ifs, const size_t offset )
{
    kvs::Vector4<T> ret{ 0, 0, 0, 0 };
    {
        ifs.seekg( offset, std::ios_base::cur );
        ifs.read( (char*)&ret.x(), sizeof(T) );
        ifs.read( (char*)&ret.y(), sizeof(T) );
        ifs.read( (char*)&ret.z(), sizeof(T) );
        ifs.read( (char*)&ret.w(), sizeof(T) );
        ifs.seekg( offset, std::ios_base::cur );
    }
    return ret;
}

template <typename T>
inline kvs::ValueArray<T> Array( std::ifstream& ifs, const size_t n, const size_t offset = 4 )
{
    kvs::ValueArray<T> ret( n );
    {
        ifs.seekg( offset, std::ios_base::cur );
        ifs.read( (char*)ret.data(), ret.byteSize() );
        ifs.seekg( offset, std::ios_base::cur );
    }
    return ret;
}

} // end of namespace Reader

} // end of namespace plot3d

} // end of namespace kvs
