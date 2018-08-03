/*****************************************************************************/
/**
 *  @file   FieldViewData.h
 *  @author Naohisa Sakamoto
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

#include <iostream>
#include <string>
#include <vector>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include <kvs/ValueArray>
#include "FVReaderTags.h"


namespace kvs
{

class FieldViewData : public kvs::FileFormatBase
{
public:

    enum ElementType
    {
        Tet = FV_TET_ELEM_ID,
        Hex = FV_HEX_ELEM_ID,
        Pri = FV_PRISM_ELEM_ID,
        Pyr = FV_PYRA_ELEM_ID
    };

    struct Version
    {
        int major; // major number
        int minor; // minor number
    };

    struct Constant
    {
        float time; // Time
        float mach; // Freestream Mach number
        float alpha; // Angle-of-attack
        float re; // Reynolds number
    };

    struct BoundaryCondition
    {
        size_t ntypes; // number of boundary face types
        std::vector<int> results; // 1: data is defined on the face, 0: not defined
        std::vector<int> normals; // normal information of the face
        std::vector<std::string> names; // face name
    };

    struct Node
    {
        float x;
        float y;
        float z;
    };

    struct Face
    {
        int type; // boundary type
        kvs::ValueArray<int> id;
    };

    struct Element
    {
        int type; // element type: 1-tet, 2-hex, 3-prism, 4-pyramid
        kvs::ValueArray<int> id;
    };

    struct Variable
    {
        int type; // 1-scalar, 3-vector (one of components of the vector)
        kvs::ValueArray<float> data; // array of the variable data
    };

    struct Grid
    {
        size_t nnodes; // number of nodes
        size_t nelements[5]; // 0-all, 1-tet, 2-hex, 3-prism, 4-pyramid
        std::vector<Node> nodes; // nodes
        std::vector<Face> faces; // boundary faces
        std::vector<Element> elements; // elements
        std::vector<Variable> variables; // variables
        std::vector<Variable> variables_on_face; // variables on the face
    };

public:

    static bool CheckExtension( const std::string& filename );

private:

    Version m_version; // version numbers
    Constant m_constant; // constant numbers
    BoundaryCondition m_boundary_condition; // boundary condition
    size_t m_nvariables; ///< number of variables
    size_t m_nvariables_on_face; ///< number of variables on the boundary face
    size_t m_ngrids; ///< number of grid data
    std::vector<std::string> m_variable_names; ///< variable names
    std::vector<std::string> m_variable_names_on_face; ///< variable names on the boundary face
    std::vector<Grid> m_grids; ///< grid data
    mutable int m_importing_element_type; ///< importing element type
    mutable size_t m_importing_grid_index; ///< importing grid index
    mutable size_t m_importing_variable_index; ///< importing variable index

public:

    FieldViewData():
        m_nvariables( 0 ),
        m_nvariables_on_face( 0 ),
        m_ngrids( 0 ) {}

    FieldViewData( const std::string& filename )
    {
        this->read( filename );
    }

    const Version& version() const { return m_version; }
    const Constant& constant() const { return m_constant; }
    const BoundaryCondition& boundaryCondition() const { return m_boundary_condition; }
    size_t numberOfVariables() const { return m_nvariables; }
    size_t numberOfVariablesOnFace() const { return m_nvariables_on_face; }
    size_t numberOfGrids() const { return m_ngrids; }
    const std::string& variableName( const int index = 0 ) const { return m_variable_names[index]; }
    const std::string& variableNameOnFace( const int index = 0 ) const { return m_variable_names_on_face[index]; }
    const Grid& grid( const int index = 0 ) const { return m_grids[index]; }

    size_t totalNumberOfNodes() const;
    size_t totalNumberOfElements( const size_t etype = 0 ) const;
    size_t totalNumberOfTetElements() const { return this->totalNumberOfElements(1); }
    size_t totalNumberOfHexElements() const { return this->totalNumberOfElements(2); }
    size_t totalNumberOfPrismElements() const { return this->totalNumberOfElements(3); }
    size_t totalNumberOfPyramidElements() const { return this->totalNumberOfElements(4); }

    int importingElementType() const { return m_importing_element_type; }
    size_t importingGridIndex() const { return m_importing_grid_index; }
    size_t importingVariableIndex() const { return m_importing_variable_index; }

    void setImportingElementType( const int etype ) const { m_importing_element_type = etype; }
    void setImportingElementTypeToAll() const { this->setImportingElementType(0); }
    void setImportingElementTypeToTet() const { this->setImportingElementType(1); }
    void setImportingElementTypeToHex() const { this->setImportingElementType(2); }
    void setImportingElementTypeToPrism() const { this->setImportingElementType(3); }
    void setImportingElementTypeToPyramid() const { this->setImportingElementType(4); }
    void setImportingGridIndex( const size_t gindex ) const { m_importing_grid_index = gindex; }
    void setImportingVariableIndex( const size_t vindex ) const { m_importing_variable_index = vindex; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool read( const std::string& gfilename, const std::string& rfilename );
    bool readAscii( const std::string& filename );
    bool readBinary( const std::string& filename );
    bool readBinary( const std::string& filename, bool swap, size_t offset );
    bool readBinaryGrid( const std::string& filename );
    bool readBinaryGrid( const std::string& filename, bool swap, size_t offset );
    bool readBinaryResult( const std::string& filename );
    bool readBinaryResult( const std::string& filename, bool swap, size_t offset );

private:

    bool write( const std::string& ) { return false; }

    void read_version( FILE* fp );
    void read_constants( FILE* fp );
    void read_grids( FILE* fp );
    void read_variable_names( FILE* fp );
    void read_nodes( FILE* fp, size_t gindex );
    void read_elements( FILE* fp, size_t gindex );
    void read_variables( FILE* fp, size_t gindex );

    void read_magic( FILE* fp, bool swap, size_t offset );
    void read_title( FILE* fp, bool swap, size_t offset );
    void read_version( FILE* fp, bool swap, size_t offset );
    void read_constants( FILE* fp, bool swap, size_t offset );
    void read_boundary_condition( FILE* fp, bool swap, size_t offset );
    void read_grids( FILE* fp, bool swap, size_t offset );
    void read_variable_names( FILE* fp, bool swap, size_t offset );
    void read_variable_names_on_face( FILE* fp, bool swap, size_t offset );
    void read_nnodes( FILE* fp, bool swap, size_t offset, size_t gindex );
    void read_nodes( FILE* fp, bool swap, size_t offset, size_t gindex );
    void read_faces( FILE* fp, bool swap, size_t offset, size_t gindex );
    void read_elements( FILE* fp, bool swap, size_t offset, size_t gindex );
    void read_variables( FILE* fp, bool swap, size_t offset, size_t gindex );
    void read_variables_on_face( FILE* fp, bool swap, size_t offset, size_t gindex );
};

} // end of namespace kvs
