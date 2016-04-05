/****************************************************************************/
/**
 *  @file   UnstructuredVolumeObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeObject.h 1787 2014-07-17 11:10:26Z naohisa.sakamoto@gmail.com $
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
 *  Unstructured volume object class.
 */
/*==========================================================================*/
class UnstructuredVolumeObject : public kvs::VolumeObjectBase
{
    kvsModule( kvs::UnstructuredVolumeObject, Object );
    kvsModuleBaseClass( kvs::VolumeObjectBase );

public:

    typedef kvs::ValueArray<kvs::UInt32> Connections;

    enum CellType
    {
        UnknownCellType = 0,  ///< Unknown cell type.
        Tetrahedra, ///< Tetrahedral cell.
        Hexahedra, ///< Hexahedral cell.
        QuadraticTetrahedra, ///< Quadratic tetrahedral cell.
        QuadraticHexahedra, ///< Quadratic Hexahedral cell.
        Pyramid, ///< Pyramidal cell.
        Point, ///< Point cell.
        Prism ///< Prism cell.
    };

private:

    CellType m_cell_type; ///< Cell type.
    size_t m_nnodes; ///< Number of nodes.
    size_t m_ncells; ///< Number of cells.
    Connections m_connections; ///< Connection ( Node ID ) array.

public:

    UnstructuredVolumeObject();

    void shallowCopy( const UnstructuredVolumeObject& object );
    void deepCopy( const UnstructuredVolumeObject& object );
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    void setCellType( CellType cell_type ) { m_cell_type = cell_type; }
    void setCellTypeToTetrahedra() { this->setCellType( Tetrahedra ); }
    void setCellTypeToHexahedra() { this->setCellType( Hexahedra ); }
    void setCellTypeToQuadraticTetrahedra() { this->setCellType( QuadraticTetrahedra ); }
    void setCellTypeToQuadraticHexahedra() { this->setCellType( QuadraticHexahedra ); }
    void setCellTypeToPyramid() { this->setCellType( Pyramid ); }
    void setCellTypeToPoint() { this->setCellType( Point ); }
    void setCellTypeToPrism() { this->setCellType( Prism ); }
    void setNumberOfNodes( const size_t nnodes ) { m_nnodes = nnodes; }
    void setNumberOfCells( const size_t ncells ) { m_ncells = ncells; }
    void setConnections( const Connections& connections ) { m_connections = connections; }

    CellType cellType() const { return m_cell_type; }
    size_t numberOfNodes() const { return m_nnodes; }
    size_t numberOfCells() const { return m_ncells; }
    const Connections& connections() const { return m_connections; }

    size_t numberOfCellNodes() const;
    void updateMinMaxCoords();

public:
    KVS_DEPRECATED( UnstructuredVolumeObject(
                        const CellType cell_type,
                        const size_t nnodes,
                        const size_t ncells,
                        const size_t veclen,
                        const Coords& coords,
                        const Connections& connections,
                        const Values& values ) )
    {
        BaseClass::setVolumeType( Unstructured );
        this->setVeclen( veclen );
        this->setCoords( coords );
        this->setValues( values );
        this->setCellType( cell_type );
        this->setNumberOfNodes( nnodes );
        this->setNumberOfCells( ncells );
        this->setConnections( connections );
    }

    KVS_DEPRECATED( void setNNodes( const size_t nnodes ) ) { this->setNumberOfNodes( nnodes ); }
    KVS_DEPRECATED( void setNCells( const size_t ncells ) ) { this->setNumberOfCells( ncells ); }
    KVS_DEPRECATED( size_t nnodes() const ) { return this->numberOfNodes(); }
    KVS_DEPRECATED( size_t ncells() const ) { return this->numberOfCells(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const UnstructuredVolumeObject& object ) );
};

} // end of namespace kvs
