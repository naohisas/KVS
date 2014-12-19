/*****************************************************************************/
/**
 *  @file   HydrogenVolumeData.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HydrogenVolumeData.h 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
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
