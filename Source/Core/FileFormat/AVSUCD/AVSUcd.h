/****************************************************************************/
/**
 *  @file   AVSUcd.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    using BaseClass = kvs::FileFormatBase;
    using Coords = kvs::ValueArray<kvs::Real32>;
    using Connections = kvs::ValueArray<kvs::UInt32>;
    using Values = kvs::ValueArray<kvs::Real32>;

    enum FormatType
    {
        FormatTypeUnknown = 0, ///< Unknown format type.
        SingleStep, ///< Single step format type ( for example tet.inp ).
        MultiStep, ///< Multi step format type ( for example ALL.inp ).
        FormatTypeSize ///< Number of format types.
    };

    enum CycleType
    {
        CycleTypeUnknown = 0, ///< Unknown cycle type.
        Data, ///< Only data changes at each step.
        Geom, ///< Only geometry changes at each step.
        DataGeom, ///< Both data and geometry changes at each step.
        CycleTypeSize ///< Number of cycle types.
    };

    enum ElementType
    {
        ElementTypeUnknown = 0, ///< Unknown element type.
        Point, ///< Point.
        Line, ///< Line.
        Triangle, ///< Triangle.
        Quadrangle, ///< Quadrangle.
        Tetrahedra, ///< Tetrahedra.
        Tetrahedra2, ///< Quadratic tetrahedra.
        Pyramid, ///< Pyramid.
        Prism, ///< Prism.
        Hexahedra, ///< Hexahedra.
        Hexahedra2, ///< Quadratic hexahedra.
        ElementTypeSize ///< Number of element types.
    };

private:
    size_t m_nsteps = 0; ///< Number of steps.
    CycleType m_cycle_type = CycleType::CycleTypeUnknown; ///< Cycle type.
    ElementType m_element_type = ElementType::ElementTypeUnknown; ///< Element type.
    size_t m_step_id = 0; ///< Step ID.
    std::string m_step_comment = ""; ///< Comment of step.
    size_t m_nnodes = 0; ///< Number of nodes.
    size_t m_nelements = 0; ///< Number of elements.
    size_t m_nvalues_per_node = 0; ///< Number of values per node.
    size_t m_ncomponents_per_node = 0; ///< Number of components per node.
    std::vector<size_t> m_veclens{}; ///< Veclens of each component.
    std::vector<std::string> m_component_names{}; ///< Names of each component.
    std::vector<std::string> m_component_units{}; ///< Units of each component.
    size_t m_component_id = 0; ///< Component ID.
    Coords m_coords{}; ///< coordinate array
    Connections m_connections{}; ///< connection array
    Values m_values{}; ///< value array

public:
    static bool CheckExtension( const std::string& filename );

public:
    AVSUcd() = default;
    virtual ~AVSUcd() = default;
    explicit AVSUcd( const std::string& filename, const size_t step_id = 0, const size_t component_id = 0 );

    size_t nsteps() const { return m_nsteps; }
    CycleType cycleType() const { return m_cycle_type; }
    ElementType elementType() const { return m_element_type; }
    size_t stepID() const { return m_step_id; }
    const std::string& stepComment() const { return m_step_comment; }
    size_t nnodes() const { return m_nnodes; }
    size_t nelements() const { return m_nelements; }
    size_t nvaluesPerNode() const { return m_nvalues_per_node; }
    size_t ncomponentsPerNode() const { return m_ncomponents_per_node; }
    const std::vector<size_t>& veclens() const { return m_veclens; }
    const std::vector<std::string>& componentNames() const { return m_component_names; }
    const std::vector<std::string>& componentUnits() const { return m_component_units; }
    size_t componentID() const { return m_component_id; }
    const std::string& componentName() const { return m_component_names[ m_component_id ]; }
    const std::string& componentUnit() const { return m_component_units[ m_component_id ]; }
    const Coords& coords() const { return m_coords; }
    const Connections& connections() const { return m_connections; }
    const Values& values() const { return m_values; }

    void setNSteps( const size_t nsteps ) { m_nsteps = nsteps; }
    void setCycleType( const CycleType cycle_type ) { m_cycle_type = cycle_type; }
    void setElementType( const ElementType element_type ) { m_element_type = element_type; }
    void setStepID( const size_t step_id ) { m_step_id = step_id; }
    void setStepComment( const std::string& step_comment ) { m_step_comment = step_comment; }
    void setNNodes( const size_t nnodes ) { m_nnodes = nnodes; }
    void setNElements( const size_t nelements ) { m_nelements = nelements; }
    void setNValuesPerNode( const size_t nvalues ) { m_nvalues_per_node = nvalues; }
    void setNComponentsPerNode( const size_t ncomponents ) { m_ncomponents_per_node = ncomponents; }
    void setVeclens( const std::vector<size_t>& veclens ) { m_veclens = veclens; }
    void setComponentNames( const std::vector<std::string>& component_names );
    void setComponentUnits( const std::vector<std::string>& component_units );
    void setComponentID( const size_t id ) { m_component_id = id; }
    void setCoords( const Coords& coords ) { m_coords = coords; }
    void setConnections( const Connections& connections ) { m_connections = connections; }
    void setValues( const Values& values ) { m_values = values; }

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
