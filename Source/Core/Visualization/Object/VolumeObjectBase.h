/****************************************************************************/
/**
 *  @file   VolumeObjectBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VolumeObjectBase.h 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <string>
#include <ostream>
#include <kvs/ObjectBase>
#include <kvs/Value>
#include <kvs/ValueArray>
#include <kvs/AnyValueArray>
#include <kvs/Math>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  VolumeObjectBase.
 */
/*==========================================================================*/
class VolumeObjectBase : public kvs::ObjectBase
{
    kvsModule( kvs::VolumeObjectBase, Object );

public:

    typedef kvs::ObjectBase BaseClass;
    typedef kvs::ValueArray<float> Coords;
    typedef kvs::AnyValueArray Values;

public:

    enum VolumeType
    {
        Structured = 0, ///< Structured volume.
        Unstructured,   ///< Unstructured volume.
        UnknownVolumeType ///< Unknow volume type.
    };

private:

    VolumeType m_volume_type; ///< volume type
    std::string m_label; ///< data label
    std::string m_unit; ///< data unit
    size_t m_veclen; ///< Vector length.
    Coords m_coords; ///< Coordinate array
    Values m_values; ///< Value array
    mutable bool m_has_min_max_values; ///< Whether includes min/max values or not
    mutable kvs::Real64 m_min_value; ///< Minimum field value
    mutable kvs::Real64 m_max_value; ///< Maximum field value

public:

    VolumeObjectBase();

    void shallowCopy( const VolumeObjectBase& object );
    void deepCopy( const VolumeObjectBase& object );
    virtual void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    void setLabel( const std::string& label ) { m_label = label; }
    void setUnit( const std::string& unit ) { m_unit = unit; }
    void setVeclen( const size_t veclen ) { m_veclen = veclen; }
    void setCoords( const Coords& coords ) { m_coords = coords; }
    void setValues( const Values& values ) { m_values = values; }
    void setMinMaxValues( const kvs::Real64 min_value, const kvs::Real64 max_value ) const;

    const std::string& label() const { return m_label; }
    const std::string& unit() const { return m_unit; }
    size_t veclen() const { return m_veclen; }
    const Coords& coords() const { return m_coords; }
    const Values& values() const { return m_values; }
    bool hasMinMaxValues() const { return m_has_min_max_values; }
    kvs::Real64 minValue() const { return m_min_value; }
    kvs::Real64 maxValue() const { return m_max_value; }

    VolumeType volumeType() const { return m_volume_type; }
    virtual size_t numberOfNodes() const = 0;
    virtual size_t numberOfCells() const = 0;
    void updateMinMaxValues() const;

protected:

    void setVolumeType( VolumeType volume_type ) { m_volume_type = volume_type; }

public:
    KVS_DEPRECATED( VolumeObjectBase(
                        const size_t veclen,
                        const Coords& coords,
                        const Values& values ) )
    {
        m_has_min_max_values = false;
        m_min_value = 0.0;
        m_max_value = 0.0;
        this->setVeclen( veclen );
        this->setCoords( coords );
        this->setValues( values );
    }

    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const VolumeObjectBase& object ) );
};

} // end of namespace kvs
