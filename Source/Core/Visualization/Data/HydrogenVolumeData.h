/*****************************************************************************/
/**
 *  @file   HydrogenVolumeData.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__HYDROGEN_VOLUME_DATA_H_INCLUDE
#define KVS__HYDROGEN_VOLUME_DATA_H_INCLUDE

#include <kvs/StructuredVolumeObject>
#include <kvs/Vector3>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Hydrogen data generator class.
 */
/*===========================================================================*/
class HydrogenVolumeData : public kvs::StructuredVolumeObject
{
public:

    typedef kvs::StructuredVolumeObject SuperClass;

public:

    HydrogenVolumeData();
    HydrogenVolumeData( const kvs::Vector3ui resolution );
    virtual ~HydrogenVolumeData();

    SuperClass* exec();
};

} // end of namespace kvs

#endif // KVS__HYDROGEN_VOLUME_DATA_H_INCLUDE
