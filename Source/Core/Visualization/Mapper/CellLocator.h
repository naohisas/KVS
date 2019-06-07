/*****************************************************************************/
/**
 *  @file   CellLocator.h
 *  @author Naohisa Sakamoto, Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <kvs/CellBase>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell locator class
 */
/*===========================================================================*/
class CellLocator
{
public:

    enum CacheMode
    {
        CacheOff = 0,
        CacheHalf = 1,
        CacheFull = 2
    };

private:

    const kvs::UnstructuredVolumeObject* m_volume; ///< reference volume
    kvs::CellBase* m_cell; ///< cell interpolator
    CacheMode m_cache_mode; ///< cache mode

public:

    CellLocator();
    virtual ~CellLocator();

public:

    void setCacheMode( const CacheMode cache_mode ){ m_cache_mode = cache_mode; }
    void setCacheModeToOff() { m_cache_mode = CacheOff; }
    void setCacheModeToHalf() { m_cache_mode = CacheHalf; }
    void setCacheModeToFull() { m_cache_mode = CacheFull; }
    void attachVolume( const kvs::UnstructuredVolumeObject* volume );

    const kvs::UnstructuredVolumeObject* volume() const { return m_volume; }
    kvs::CellBase* cell() const { return m_cell; }
    CacheMode cacheMode() const { return m_cache_mode; }

    virtual void build() = 0;
    virtual int findCell( const kvs::Vec3 p ) = 0;
    virtual void clearCache() = 0;
};

} // end of namespace kvs
