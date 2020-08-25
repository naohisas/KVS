/*****************************************************************************/
/**
 *  @file   TornadoVolumeData.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__TORNADO_VOLUME_DATA_H_INCLUDE
#define KVS__TORNADO_VOLUME_DATA_H_INCLUDE

#include <kvs/StructuredVolumeObject>
#include <kvs/Vector3>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Tornado data generator class.
 */
/*===========================================================================*/
class TornadoVolumeData : public kvs::StructuredVolumeObject
{
public:

    typedef kvs::StructuredVolumeObject SuperClass;

private:

    int m_time; ///< time value

public:

    TornadoVolumeData();
    TornadoVolumeData( const kvs::Vector3ui resolution, const int time = 0 );
    virtual ~TornadoVolumeData();

    int time() const;
    void setTime( const int time );

    SuperClass* exec();
};

} // end of namespace kvs

#endif // KVS__TORNADO_VOLUME_DATA_H_INCLUDE
