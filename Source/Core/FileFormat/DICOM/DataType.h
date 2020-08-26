/****************************************************************************/
/**
 *  @file   DataType.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__DCM__DATA_TYPE_H_INCLUDE
#define KVS__DCM__DATA_TYPE_H_INCLUDE

#include <string>

namespace kvs
{

namespace dcm
{

enum DataType
{
    DATA_CHAR   = 0, ///< char type data
    DATA_UCHAR  = 1, ///< unsigned char type data
    DATA_INT    = 2, ///< int type data
    DATA_UINT   = 3, ///< unsigned int type data
    DATA_SHORT  = 4, ///< short type data
    DATA_USHORT = 5, ///< unsigned short type data
    DATA_FLOAT  = 6, ///< float type data
    DATA_DOUBLE = 7, ///< double type data
    DATA_STRING = 8, ///< string type data
    DATA_OTHER  = 9, ///< other
    NUMBER_OF_DATA_TYPES = 10
};

const std::string DATA_TYPE_TO_STRING[NUMBER_OF_DATA_TYPES] =
{
    "char",
    "unsigned char",
    "int",
    "unsigned int",
    "short",
    "unsigned short",
    "float",
    "double",
    "string",
    "other"
};

const unsigned int DATA_TYPE_TO_SIZE[NUMBER_OF_DATA_TYPES] =
{
    sizeof(char),
    sizeof(unsigned char),
    sizeof(int),
    sizeof(unsigned int),
    sizeof(short),
    sizeof(unsigned short),
    sizeof(float),
    sizeof(double),
    0, // cannot decide
    0  // cannot decide
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM_DATA_TYPE_H_INCLUDE
