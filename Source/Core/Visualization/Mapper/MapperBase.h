/****************************************************************************/
/**
 *  @file   MapperBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MapperBase.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MAPPER_BASE_H_INCLUDE
#define KVS__MAPPER_BASE_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/VolumeObjectBase>
#include <kvs/TransferFunction>
#include <kvs/ColorMap>
#include <kvs/OpacityMap>
#include <kvs/Module>


namespace kvs
{

class VolumeObjectBase;

/*==========================================================================*/
/**
 *  MapperBase.
 */
/*==========================================================================*/
class MapperBase
{
    kvsModuleBase( kvs::MapperBase );

private:

    kvs::TransferFunction m_transfer_function; ///< Transfer function.
    const kvs::VolumeObjectBase* m_volume; ///< Volume object.
    bool m_is_success; ///< Check flag for mapping.

public:

    MapperBase();
    explicit MapperBase( const kvs::TransferFunction& transfer_function );
    virtual ~MapperBase();

    virtual kvs::ObjectBase* exec( const kvs::ObjectBase* object ) = 0;

    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    void setColorMap( const kvs::ColorMap& color_map );
    void setOpacityMap( const kvs::OpacityMap& opacity_map );

    const kvs::VolumeObjectBase* const volume() const;
    const kvs::TransferFunction& transferFunction() const;
    const kvs::ColorMap& colorMap() const;
    const kvs::OpacityMap& opacityMap() const;
    bool isSuccess() const;
    bool isFailure() const;

protected:

    void attachVolume( const kvs::VolumeObjectBase* volume );
    void setRange( const kvs::VolumeObjectBase* volume );
    void setMinMaxCoords( const kvs::VolumeObjectBase* volume, kvs::ObjectBase* object );
    void setSuccess( const bool success );

protected:
    KVS_DEPRECATED( void attach_volume( const kvs::VolumeObjectBase* volume ) ) { this->attachVolume( volume ); }
    KVS_DEPRECATED( void set_range( const kvs::VolumeObjectBase* volume ) ) { this->setRange( volume ); }
    KVS_DEPRECATED( void set_min_max_coords( const kvs::VolumeObjectBase* volume, kvs::ObjectBase* object ) ) { this->setMinMaxCoords( volume, object ); }
};

} // end of namespace kvs

#endif // KVS__MAPPER_BASE_H_INCLUDE
