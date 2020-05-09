/****************************************************************************/
/**
 *  @file   StructuredVolumeObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeObject.h 1787 2014-07-17 11:10:26Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <ostream>
#include <kvs/Module>
#include <kvs/VolumeObjectBase>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  StructuredVolumeObject.
 */
/*==========================================================================*/
class StructuredVolumeObject : public kvs::VolumeObjectBase
{
    kvsModule( kvs::StructuredVolumeObject, Object );
    kvsModuleBaseClass( kvs::VolumeObjectBase );

public:
    enum GridType
    {
        UnknownGridType = 0, ///< Unknown grid type.
        Uniform,             ///< Uniform grid.
        Rectilinear,         ///< Rectilinear grid.
        Curvilinear,         ///< Curvilinear grid.
    };

private:
    GridType m_grid_type; ///< grid type
    kvs::Vec3ui m_resolution; ///< Node resolution.

public:
    StructuredVolumeObject();

    void shallowCopy( const StructuredVolumeObject& object );
    void deepCopy( const StructuredVolumeObject& object );
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    void setGridType( GridType grid_type ) { m_grid_type = grid_type; }
    void setGridTypeToUniform() { this->setGridType( Uniform ); }
    void setGridTypeToRectilinear() { this->setGridType( Rectilinear ); }
    void setGridTypeToCurvilinear() { this->setGridType( Curvilinear ); }
    void setResolution( const kvs::Vec3ui& resolution ) { m_resolution = resolution; }

    GridType gridType() const { return m_grid_type; }
    const kvs::Vec3ui& resolution() const { return m_resolution; }
    size_t numberOfNodesPerLine() const;
    size_t numberOfNodesPerSlice() const;
    size_t numberOfNodes() const;
    size_t numberOfCells() const;

    void updateMinMaxCoords();

public:
    KVS_DEPRECATED( StructuredVolumeObject(
                        const kvs::Vector3ui& resolution,
                        const size_t veclen,
                        const Values& values ) )
    {
        BaseClass::setVolumeType( Structured );
        this->setVeclen( veclen );
        this->setValues( values );
        this->setGridType( Uniform );
        this->setResolution( resolution );
    }

    KVS_DEPRECATED( StructuredVolumeObject(
                        const GridType grid_type,
                        const kvs::Vector3ui& resolution,
                        const size_t veclen,
                        const Coords& coords,
                        const Values& values ) )
    {
        BaseClass::setVolumeType( Structured );
        this->setVeclen( veclen );
        this->setCoords( coords );
        this->setValues( values );
        this->setGridType( grid_type );
        this->setResolution( resolution );
    }

    KVS_DEPRECATED( size_t nnodesPerLine() const ) { return this->numberOfNodesPerLine(); }
    KVS_DEPRECATED( size_t nnodesPerSlice() const ) { return this->numberOfNodesPerSlice(); }
    KVS_DEPRECATED( size_t nnodes() const ) { return this->numberOfNodes(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const StructuredVolumeObject& object ) );
};

} // end of namespace kvs
