/****************************************************************************/
/**
 *  @file AVSUcd.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AVSUcd.h 1454 2013-03-23 05:59:35Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__AVS_UCD_H_INCLUDE
#define KVS__AVS_UCD_H_INCLUDE

#include <vector>
#include <string>
#include <kvs/FileFormatBase>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>


namespace kvs
{

/*==========================================================================*/
/**
 *  AVS UCD ( Unstructured Cell Data ) class.
 */
/*==========================================================================*/
class AVSUcd : public FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef kvs::ValueArray<kvs::Real32> Coords;
    typedef kvs::ValueArray<kvs::UInt32> Connections;
    typedef kvs::ValueArray<kvs::Real32> Values;

    enum FormatType
    {
        FormatTypeUnknown = 0, ///< Unknown format type.
        SingleStep,            ///< Single step format type ( for example tet.inp ).
        MultiStep,             ///< Multi step format type ( for example ALL.inp ).
        FormatTypeSize         ///< Number of format types.
    };

    enum CycleType
    {
        CycleTypeUnknown = 0, ///< Unknown cycle type.
        Data,                 ///< Only data changes at each step.
        Geom,                 ///< Only geometry changes at each step.
        DataGeom,             ///< Both data and geometry changes at each step.
        CycleTypeSize         ///< Number of cycle types.
    };

    enum ElementType
    {
        ElementTypeUnknown = 0, ///< Unknown element type.
        Point,                  ///< Point.
        Line,                   ///< Line.
        Triangle,               ///< Triangle.
        Quadrangle,             ///< Quadrangle.
        Tetrahedra,             ///< Tetrahedra.
        Tetrahedra2,            ///< Quadratic tetrahedra.
        Pyramid,                ///< Pyramid.
        Prism,                  ///< Prism.
        Hexahedra,              ///< Hexahedra.
        Hexahedra2,             ///< Quadratic hexahedra.
        ElementTypeSize         ///< Number of element types.
    };

private:

    size_t m_nsteps; ///< Number of steps.
    CycleType m_cycle_type; ///< Cycle type.
    ElementType m_element_type; ///< Element type.
    size_t m_step_id; ///< Step ID.
    std::string m_step_comment; ///< Comment of step.
    size_t m_nnodes; ///< Number of nodes.
    size_t m_nelements; ///< Number of elements.
    size_t m_nvalues_per_node; ///< Number of values per node.
    size_t m_ncomponents_per_node; ///< Number of components per node.
    std::vector<size_t> m_veclens; ///< Veclens of each component.
    std::vector<std::string> m_component_names; ///< Names of each component.
    std::vector<std::string> m_component_units; ///< Units of each component.
    size_t m_component_id; ///< Component ID.
    Coords m_coords; ///< coordinate array
    Connections m_connections; ///< connection array
    Values m_values; ///< value array

public:

    static bool CheckExtension( const std::string& filename );

public:

    AVSUcd();
    explicit AVSUcd( const std::string& filename, const size_t step_id = 0, const size_t component_id = 0 );
    virtual ~AVSUcd();

    size_t nsteps() const;
    CycleType cycleType() const;
    ElementType elementType() const;
    size_t stepID() const;
    const std::string& stepComment() const;
    size_t nnodes() const;
    size_t nelements() const;
    size_t nvaluesPerNode() const;
    size_t ncomponentsPerNode() const;
    const std::vector<size_t>& veclens() const;
    const std::vector<std::string>& componentNames() const;
    const std::vector<std::string>& componentUnits() const;
    size_t componentID() const;
    const std::string& componentName() const;
    const std::string& componentUnit() const;
    const Coords& coords() const;
    const Connections& connections() const;
    const Values& values() const;

    void setNSteps( const size_t nsteps );
    void setCycleType( const CycleType cycle_type );
    void setElementType( const ElementType element_type );
    void setStepID( const size_t step_id );
    void setStepComment( const std::string& step_comment );
    void setNNodes( const size_t nnodes );
    void setNElements( const size_t nelements );
    void setNValuesPerNode( const size_t nvalues_per_node );
    void setNComponentsPerNode( const size_t ncomponents_per_node );
    void setVeclens( const std::vector<size_t>& veclens );
    void setComponentNames( const std::vector<std::string>& component_names );
    void setComponentUnits( const std::vector<std::string>& component_units );
    void setComponentID( const size_t component_id );
    void setCoords( const Coords& coords );
    void setConnections( const Connections& connections );
    void setValues( const Values& values );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    void read_control_file( FILE* const ifs );
    void read_binary_files( const std::vector<std::string>& filenames );
    void read_binary_file( const std::string& filename );

    void read_single_step_format( FILE* const ifs );
    void read_multi_step_format( FILE* const ifs );
    void read_multi_step_format_data( FILE* const ifs );
    void read_multi_step_format_geom( FILE* const ifs );
    void read_multi_step_format_data_geom( FILE* const ifs );
    void seek_target_step( FILE* const ifs );
    void read_coords( FILE* const ifs );
    void read_connections( FILE* const ifs );
    void read_components( FILE* const ifs );
    void read_values( FILE* const ifs );
    void write_single_step_format( FILE* const ofs ) const;
    void write_coords( FILE* const ofs ) const;
    void write_connections( FILE* const ofs ) const;
    void write_components( FILE* const ofs ) const;
    void write_values( FILE* const ofs ) const;
};

} // end of namespace kvs

#endif // KVS__AVS_UCD_H_INCLUDE
