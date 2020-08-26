/****************************************************************************/
/**
 *  @file   Attribute.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__DCM__ATTRIBUTE_H_INCLUDE
#define KVS__DCM__ATTRIBUTE_H_INCLUDE

#include <fstream>
#include <string>


namespace kvs
{

namespace dcm
{

/*===========================================================================*/
/**
 *  @brief  DICOM attribute class.
 */
/*===========================================================================*/
class Attribute
{
protected:

    bool m_part10; ///< part10 flag
    bool m_swap;   ///< swap flag

public:

    Attribute( void );
    virtual ~Attribute( void );

public:

    bool part10( void ) const;
    bool swap( void ) const;

public:

    Attribute& operator = ( const Attribute& a );

public:

    bool check( std::ifstream& ifs );
};

} // end of namespace dcm

} // end of namespace kvs

#endif // KVS__DCM__ATTRIBUTE_H_INCLUDE
