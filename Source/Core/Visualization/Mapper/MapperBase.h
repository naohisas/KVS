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
#pragma once
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
    kvs::TransferFunction m_tfunc; ///< Transfer function.
    const kvs::VolumeObjectBase* m_volume; ///< Volume object.
    bool m_is_success; ///< Check flag for mapping.

public:
    MapperBase();
    explicit MapperBase( const kvs::TransferFunction& transfer_function );
    virtual ~MapperBase();

    virtual kvs::ObjectBase* exec( const kvs::ObjectBase* object ) = 0;

    void setTransferFunction( const kvs::TransferFunction& tfunc ) { m_tfunc = tfunc; }
    void setColorMap( const kvs::ColorMap& cmap ) { m_tfunc.setColorMap( cmap ); }
    void setOpacityMap( const kvs::OpacityMap& omap ) { m_tfunc.setOpacityMap( omap ); }

    const kvs::VolumeObjectBase* volume() const { return m_volume; }
    const kvs::TransferFunction& transferFunction() const { return m_tfunc; }
    const kvs::ColorMap& colorMap() const { return m_tfunc.colorMap(); }
    const kvs::OpacityMap& opacityMap() const { return m_tfunc.opacityMap(); }
    bool isSuccess() const { return m_is_success; }
    bool isFailure() const { return !m_is_success; }

protected:
    void attachVolume( const kvs::VolumeObjectBase* volume ) { m_volume = volume; }
    void setSuccess( const bool success ) { m_is_success = success; }
    void setRange( const kvs::VolumeObjectBase* volume );
    void setMinMaxCoords( const kvs::VolumeObjectBase* volume, kvs::ObjectBase* object );

protected:
    KVS_DEPRECATED( void attach_volume( const kvs::VolumeObjectBase* volume ) ) { this->attachVolume( volume ); }
    KVS_DEPRECATED( void set_range( const kvs::VolumeObjectBase* volume ) ) { this->setRange( volume ); }
    KVS_DEPRECATED( void set_min_max_coords( const kvs::VolumeObjectBase* volume, kvs::ObjectBase* object ) ) { this->setMinMaxCoords( volume, object ); }
};

} // end of namespace kvs
