/*****************************************************************************/
/**
 *  @file   TableObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <ostream>
#include <kvs/Module>
#include <kvs/ObjectBase>
#include <kvs/Type>
#include <kvs/AnyValueArray>
#include <kvs/AnyValueTable>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  TableObject class.
 */
/*===========================================================================*/
class TableObject : public kvs::ObjectBase
{
    kvsModule( kvs::TableObject, Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    using Columns = kvs::AnyValueTable::Columns;
    using Labels = std::vector<std::string>;
    using Values = std::vector<kvs::Real64>;
    using InsideRangeFlags = std::vector<kvs::UInt8>;

private:
    size_t m_nrows = 0; ///< number of rows
    size_t m_ncolumns = 0; ///< number of columns
    kvs::AnyValueTable m_table{}; ///< table data
    Labels m_labels{}; ///< label list
    Values m_min_values{}; ///< min. values for each column
    Values m_max_values{}; ///< max. values for each column
    Values m_min_ranges{}; ///< min. value range
    Values m_max_ranges{}; ///< max. value range
    InsideRangeFlags m_inside_range_flags{}; ///< check flags for value range

public:
    TableObject(): BaseClass( Table ) {}
    virtual ~TableObject() = default;

    void shallowCopy( const TableObject& other );
    void deepCopy( const TableObject& other );
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    void addColumn( const kvs::AnyValueArray& array, const std::string& label = "" );
    void addColumn( const kvs::AnyValueArray& array, const kvs::Real64 min_value, const kvs::Real64 max_value, const std::string& label = "" );
    void setTable( const kvs::AnyValueTable& table, const Labels& lanels = Labels() );
    void setMinValue( const size_t column_index, const kvs::Real64 value );
    void setMaxValue( const size_t column_index, const kvs::Real64 value );
    void setMinMaxValues( const size_t column_index, const kvs::Real64 min_value, const kvs::Real64 max_value );
    void setMinRange( const size_t column_index, const kvs::Real64 range );
    void setMaxRange( const size_t column_index, const kvs::Real64 range );
    void setRange( const size_t column_index, const kvs::Real64 min_range, const kvs::Real64 max_range );
    void moveMinRange( const size_t column_index, const kvs::Real64 drange );
    void moveMaxRange( const size_t column_index, const kvs::Real64 drange );
    void moveRange( const size_t column_index, const kvs::Real64 drange );
    void resetRange( const size_t column_index );
    void resetRange();

    size_t numberOfColumns() const { return m_ncolumns; }
    size_t numberOfRows() const { return m_nrows; }
    const kvs::AnyValueTable& table() const { return m_table; }
    const Labels& labels() const { return m_labels; }
    const Columns& columns() const { return m_table.columns(); }
    const Values& minValues() const { return m_min_values; }
    const Values& maxValues() const { return m_max_values; }
    const Values& minRanges() const { return m_min_ranges; }
    const Values& maxRanges() const { return m_max_ranges; }
    const InsideRangeFlags& insideRangeFlags() const { return m_inside_range_flags; }
    const std::string& label( const size_t index ) const { return m_labels[index];  }
    const kvs::AnyValueArray& column( const size_t index ) const { return m_table.column(index); }
    kvs::Real64 minValue( const size_t index ) const { return m_min_values[index]; }
    kvs::Real64 maxValue( const size_t index ) const { return m_max_values[index]; }
    kvs::Real64 minRange( const size_t column_index ) const { return m_min_ranges[column_index]; }
    kvs::Real64 maxRange( const size_t column_index ) const { return m_max_ranges[column_index]; }
    bool insideRange( const size_t row_index ) const { return m_inside_range_flags[row_index] == 1; }
    template <typename T> T at( const size_t row, const size_t col ) const { return m_table.column(col).at<T>(row); }

protected:
    void setNumberOfRows( const size_t nrows ) { m_nrows = nrows; }
    void setNumberOfColumns( const size_t ncolumns ) { m_ncolumns = ncolumns; }
    void setLabels( const Labels& labels ) { m_labels = labels; }
    void setMinValues( const Values& min_values ) { m_min_values = min_values; }
    void setMaxValues( const Values& max_values ) { m_max_values = max_values; }
    void setMinRanges( const Values& min_ranges ) { m_min_ranges = min_ranges; }
    void setMaxRanges( const Values& max_ranges ) { m_max_ranges = max_ranges; }
    void setInsideRangeFlags( const InsideRangeFlags& inside_range_flags ) { m_inside_range_flags = inside_range_flags; }

public:
    typedef KVS_DEPRECATED( std::vector<std::string> LabelList );
    typedef KVS_DEPRECATED( std::vector<kvs::AnyValueArray> ColumnList );
    typedef KVS_DEPRECATED( std::vector<kvs::Real64> ValueList );
    typedef KVS_DEPRECATED( std::vector<kvs::UInt8> RangeList );
    KVS_DEPRECATED( size_t ncolumns() const ) { return this->numberOfColumns(); }
    KVS_DEPRECATED( size_t nrows() const ) { return this->numberOfRows(); }
    KVS_DEPRECATED( const Labels labelList() const ) { return this->labels(); }
    KVS_DEPRECATED( const Columns columnList() const ) { return this->table().columns(); }
    KVS_DEPRECATED( const Values minValueList() const ) { return this->minValues(); }
    KVS_DEPRECATED( const Values maxValueList() const ) { return this->maxValues(); }
    KVS_DEPRECATED( const Values& minRangeList() const ) { return this->minRanges(); }
    KVS_DEPRECATED( const Values& maxRangeList() const ) { return this->maxRanges(); }
    KVS_DEPRECATED( const InsideRangeFlags& insideRangeList() const ) { return this->insideRangeFlags(); }
};

} // end of namespace kvs
