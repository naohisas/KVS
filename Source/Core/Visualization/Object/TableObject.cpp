/*****************************************************************************/
/**
 *  @file   TableObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TableObject.cpp 1822 2014-10-24 09:01:51Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TableObject.h"
#include <kvs/Value>
#include <kvs/Math>
#include <kvs/KVSMLTableObject>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLTableObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLTableObject::ExternalAscii; }
        else { return kvs::KVSMLTableObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLTableObject::ExternalBinary;
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TableObject class.
 */
/*===========================================================================*/
TableObject::TableObject()
{
    BaseClass::setObjectType( Table );
    m_nrows = 0;
    m_ncolumns = 0;
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  other [in] table object
 */
/*===========================================================================*/
void TableObject::shallowCopy( const TableObject& other )
{
    BaseClass::operator=( other );
    this->m_nrows = other.numberOfRows();
    this->m_ncolumns = other.numberOfColumns();
    this->m_labels = other.labels();
    this->m_table = other.table();
    this->m_min_values = other.minValues();
    this->m_max_values = other.maxValues();
    this->m_min_ranges = other.minRanges();
    this->m_max_ranges = other.maxRanges();
    this->m_inside_range_flags = other.insideRangeFlags();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  other [in] table object
 */
/*===========================================================================*/
void TableObject::deepCopy( const TableObject& other )
{
    { m_table.clear(); kvs::AnyValueTable().swap( m_table ); }
    { m_labels.clear(); Labels().swap( m_labels ); }
    { m_min_values.clear(); Values().swap( m_min_values ); }
    { m_max_values.clear(); Values().swap( m_max_values ); }
    { m_min_ranges.clear(); Values().swap( m_min_ranges ); }
    { m_max_ranges.clear(); Values().swap( m_max_ranges ); }
    { m_inside_range_flags.clear(); InsideRangeFlags().swap( m_inside_range_flags ); }

    BaseClass::operator=( other );
    this->m_nrows = other.numberOfRows();
    this->m_ncolumns = other.numberOfColumns();
    for ( size_t i = 0; i < m_table.columnSize(); i++ ) this->m_table.pushBackColumn( other.column(i).clone() );
    for ( size_t i = 0; i < m_labels.size(); i++ ) this->m_labels.push_back( other.label(i) );
    for ( size_t i = 0; i < m_min_values.size(); i++ ) this->m_min_values.push_back( other.minValue(i) );
    for ( size_t i = 0; i < m_max_values.size(); i++ ) this->m_max_values.push_back( other.maxValue(i) );
    for ( size_t i = 0; i < m_min_ranges.size(); i++ ) this->m_min_ranges.push_back( other.minRange(i) );
    for ( size_t i = 0; i < m_max_ranges.size(); i++ ) this->m_max_ranges.push_back( other.maxRange(i) );
    for ( size_t i = 0; i < m_inside_range_flags.size(); i++ ) this->m_inside_range_flags.push_back( other.insideRange(i) );
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the table object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void TableObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object type : " << "table object" << std::endl;
    os << indent << "Number of columns : " << this->numberOfColumns() << std::endl;
    os << indent << "Number of rows : " << this->numberOfRows() << std::endl;
    os << indent << "Labels for each column : ";
    for ( size_t i = 0; i < this->labels().size(); i++ ) { os << "\"" << this->labels()[i] << "\", "; } os << std::endl;
    os << indent << "Min. values for each column : ";
    for ( size_t i = 0; i < this->minValues().size(); i++ ) { os << this->minValues()[i] << ", "; } os << std::endl;
    os << indent << "Max. values for each column : ";
    for ( size_t i = 0; i < this->maxValues().size(); i++ ) { os << this->maxValues()[i] << ", "; } os << std::endl;
    os << indent << "Min. ranges for each column : ";
    for ( size_t i = 0; i < this->minRanges().size(); i++ ) { os << this->minRanges()[i] << ", "; } os << std::endl;
    os << indent << "Max. ranges for each column : ";
    for ( size_t i = 0; i < this->maxRanges().size(); i++ ) { os << this->maxRanges()[i] << ", "; } os << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a table object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool TableObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLTableObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not a table object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLTableObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    const size_t ncolumns = kvsml.ncolumns();
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        const std::string label = kvsml.labelList().at(i);
        const kvs::AnyValueArray& column = kvsml.columnList().at(i);
        this->addColumn( column, label );

        if ( kvsml.hasMinValueList().at(i) ) { this->setMinValue( i, kvsml.minValueList().at(i) ); }
        if ( kvsml.hasMaxValueList().at(i) ) { this->setMaxValue( i, kvsml.maxValueList().at(i) ); }
        if ( kvsml.hasMinRangeList().at(i) ) { this->setMinRange( i, kvsml.minRangeList().at(i) ); }
        if ( kvsml.hasMaxRangeList().at(i) ) { this->setMaxRange( i, kvsml.maxRangeList().at(i) ); }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the table object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool TableObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLTableObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );

    const size_t ncolumns = this->numberOfColumns();
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        const std::string label = this->labels().at(i);
        const kvs::AnyValueArray& column = this->column(i);
        kvsml.addColumn( column, label );
    }

    return kvsml.write( filename );
}

/*===========================================================================*/
/**
 *  @brief  Adds a column.
 *  @param  array [in] column array
 *  @param  label [in] column label
 */
/*===========================================================================*/
void TableObject::addColumn( const kvs::AnyValueArray& array, const std::string& label )
{
    m_ncolumns++;
    m_nrows = kvs::Math::Max( m_nrows, array.size() );

    m_table.pushBackColumn( array );
    m_labels.push_back( label );

    kvs::Real64 min_value = kvs::Value<kvs::Real64>::Max();
    kvs::Real64 max_value = kvs::Value<kvs::Real64>::Min();
    const std::type_info& type = array.typeInfo()->type();
    if ( type == typeid( kvs::Int8 ) )
    {
        const kvs::Int8* value = static_cast<const kvs::Int8*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::UInt8 ) )
    {
        const kvs::UInt8* value = static_cast<const kvs::UInt8*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        const kvs::Int16* value = static_cast<const kvs::Int16*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        const kvs::UInt16* value = static_cast<const kvs::UInt16*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::Int32 ) )
    {
        const kvs::Int32* value = static_cast<const kvs::Int32*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::UInt32 ) )
    {
        const kvs::UInt32* value = static_cast<const kvs::UInt32*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::Int64 ) )
    {
        const kvs::Int64* value = static_cast<const kvs::Int64*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::UInt64 ) )
    {
        const kvs::UInt64* value = static_cast<const kvs::UInt64*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::Real32 ) )
    {
        const kvs::Real32* value = static_cast<const kvs::Real32*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( kvs::Real64 ) )
    {
        const kvs::Real64* value = static_cast<const kvs::Real64*>( array.data() );
        for( size_t i = 0; i < array.size(); i++ )
        {
            min_value = kvs::Math::Min( min_value, kvs::Real64( value[i] ) );
            max_value = kvs::Math::Max( max_value, kvs::Real64( value[i] ) );
        }
    }
    else if ( type == typeid( std::string ) )
    {
        min_value = 0.0;
        max_value = 0.0;
    }

    m_min_values.push_back( min_value );
    m_max_values.push_back( max_value );
    m_min_ranges.push_back( min_value );
    m_max_ranges.push_back( max_value );
    m_inside_range_flags.resize( m_nrows, 1 );
}

/*===========================================================================*/
/**
 *  @brief  Sets table data.
 *  @param  table [in] table data
 *  @param  labels [in] label data
 */
/*===========================================================================*/
void TableObject::setTable( const kvs::AnyValueTable& table, const Labels& labels )
{
    { m_table.clear(); kvs::AnyValueTable().swap( m_table ); }
    { m_labels.clear(); Labels().swap( m_labels ); }
    { m_min_values.clear(); Values().swap( m_min_values ); }
    { m_max_values.clear(); Values().swap( m_max_values ); }
    { m_min_ranges.clear(); Values().swap( m_min_ranges ); }
    { m_max_ranges.clear(); Values().swap( m_max_ranges ); }
    { m_inside_range_flags.clear(); InsideRangeFlags().swap( m_inside_range_flags ); }

    for ( size_t i = 0; i < table.columnSize(); i++ )
    {
        const std::string label = i < labels.size() ? labels[i] : "";
        this->addColumn( table.column(i), label );
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets minimum value to the specified column.
 *  @param  column_index [in] column index
 *  @param  value [in] minimum value
 */
/*===========================================================================*/
void TableObject::setMinValue( const size_t column_index, const kvs::Real64 value )
{
    if ( value > m_min_ranges[column_index] ) { this->setMinRange( column_index, value ); }
    m_min_values[column_index] = value;
}

/*===========================================================================*/
/**
 *  @brief  Sets maximum value to the specified column.
 *  @param  column_index [in] column index
 *  @param  value [in] maximum value
 */
/*===========================================================================*/
void TableObject::setMaxValue( const size_t column_index, const kvs::Real64 value )
{
    if ( value < m_max_ranges[column_index] ) { this->setMaxRange( column_index, value ); }
    m_max_values[column_index] = value;
}

/*===========================================================================*/
/**
 *  @brief  Sets minimum range value to the specified column.
 *  @param  column_index [in] column index
 *  @param  range [in] range value
 */
/*===========================================================================*/
void TableObject::setMinRange( const size_t column_index, const kvs::Real64 range )
{
    const kvs::Real64 min_value = this->minValue(column_index);
    const kvs::Real64 max_range = m_max_ranges[column_index];

    const kvs::Real64 min_range_old = m_min_ranges[column_index];
    const kvs::Real64 min_range_new = kvs::Math::Clamp( range, min_value, max_range );

    if ( kvs::Math::Equal( min_range_old, min_range_new ) ) return;
    m_min_ranges[column_index] = min_range_new;

    const size_t nrows = this->numberOfRows();
    const size_t ncolumns = this->numberOfColumns();
    if ( m_table.columns().size() > 0 )
    {
        if ( min_range_new > min_range_old )
        {
            /* In case of flags turned off, you just have to check whether the value
             * in the specified column is included in the range.
             *
             *  (before) |xxx+oooooooo*xxxxxx|  o: on, x: off, +: min_range, *: max_range
             *  (after)  |xxxAxxxxBooo*xxxxxx|  A: min_range_old, B: min_range_new
             */
            const kvs::AnyValueArray& column = this->column( column_index );
            for ( size_t i = 0; i < nrows; i++ )
            {
                const kvs::Real64 value = column[i].to<kvs::Real64>();
                if (  min_range_old <= value && value <= min_range_new )
                {
                    m_inside_range_flags[i] = 0;
                }
            }
        }
        else
        {
            /* In case of flags turned on, you have to check the all of colums.
             *
             *  (before) |xxxxxxxx+ooo*xxxxxx|  o: on, x: off, +: min_range, *: max_range
             *  (after)  |xxxAooooBooo*xxxxxx|  A: min_range, B: min_range_old
             */
            for ( size_t i = 0; i < nrows; i++ )
            {
                m_inside_range_flags[i] = 1;
                for ( size_t j = 0; j < ncolumns; j++ )
                {
                    const kvs::AnyValueArray& column = this->column( j );
                    const kvs::Real64 value = column[i].to<kvs::Real64>();
                    if (  !( m_min_ranges[j] <= value && value <= m_max_ranges[j] ) )
                    {
                        m_inside_range_flags[i] = 0;
                        break;
                    }
                }
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets maximum range value to the specified column.
 *  @param  column_index [in] column index
 *  @param  range [in] range value
 */
/*===========================================================================*/
void TableObject::setMaxRange( const size_t column_index, const kvs::Real64 range )
{
    const kvs::Real64 min_range = m_min_ranges[column_index];
    const kvs::Real64 max_value = this->maxValue(column_index);

    const kvs::Real64 max_range_old = m_max_ranges[column_index];
    const kvs::Real64 max_range_new = kvs::Math::Clamp( range, min_range, max_value );

    if ( kvs::Math::Equal( max_range_old, max_range_new ) ) return;
    m_max_ranges[column_index] = max_range_new;

    const size_t nrows = this->numberOfRows();
    const size_t ncolumns = this->numberOfColumns();
    if ( m_table.columns().size() > 0 )
    {
        if ( max_range_new > max_range_old )
        {
            /* In case of flags turned on, you have to check the all of colums.
             *
             *  (before) |xxx*oooooooo+xxxxxx|  o: on, x: off, *: min_range, +: max_range
             *  (after)  |xxx*ooooooooAoooBxx|  A: max_range_old, B: max_range_new
             */
            for ( size_t i = 0; i < nrows; i++ )
            {
                m_inside_range_flags[i] = 1;
                for ( size_t j = 0; j < ncolumns; j++ )
                {
                    const kvs::AnyValueArray& column = this->column( j );
                    const kvs::Real64 value = column[i].to<kvs::Real64>();
                    if (  !( m_min_ranges[j] <= value && value <= m_max_ranges[j] ) )
                    {
                        m_inside_range_flags[i] = 0;
                        break;
                    }
                }
            }
        }
        else
        {
            /* In case of flags turned off, you just have to check whether the value
             * in the specified column is included in the range.
             *
             *  (before) |xxx*oooooooo+xxxxxx|  o: on, x: off, *: min_range, +: max_range
             *  (after)  |xxx*ooooBxxxAxxxxxx|  A: max_range_old, B: max_range_new
             */
            const kvs::AnyValueArray& column = this->column( column_index );
            for ( size_t i = 0; i < nrows; i++ )
            {
                const kvs::Real64 value = column[i].to<kvs::Real64>();
                if (  max_range_new <= value && value <= max_range_old )
                {
                    m_inside_range_flags[i] = 0;
                }
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets minimum and maximum range values to the specified column.
 *  @param  column_index [in] column index
 *  @param  min_range [in] minimum range value
 *  @param  max_range [in] maximum range value
 */
/*===========================================================================*/
void TableObject::setRange( const size_t column_index, const kvs::Real64 min_range, const kvs::Real64 max_range )
{
    this->setMinRange( column_index, min_range );
    this->setMaxRange( column_index, max_range );
}

/*===========================================================================*/
/**
 *  @brief  Moves minimum range value to the specified column.
 *  @param  column_index [in] column index
 *  @param  drange [in] distance of range value
 */
/*===========================================================================*/
void TableObject::moveMinRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 min_range = this->minRange( column_index );
    this->setMinRange( column_index, min_range + drange );
}

/*===========================================================================*/
/**
 *  @brief  Moves maximum range value to the specified column.
 *  @param  column_index [in] column index
 *  @param  drange [in] distance of range value
 */
/*===========================================================================*/
void TableObject::moveMaxRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 max_range = this->maxRange( column_index );
    this->setMaxRange( column_index, max_range + drange );
}

/*===========================================================================*/
/**
 *  @brief  Moves maximum range value to the specified column.
 *  @param  column_index [in] column index
 *  @param  drange [in] distance of range value
 */
/*===========================================================================*/
void TableObject::moveRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 min_range = this->minRange( column_index );
    const kvs::Real64 max_range = this->maxRange( column_index );
    const kvs::Real64 min_value = this->minValue( column_index );
    const kvs::Real64 max_value = this->maxValue( column_index );
    const kvs::Real64 range_width = max_range - min_range;

    if ( max_range + drange > max_value )
    {
        this->setMinRange( column_index, max_value - range_width );
        this->setMaxRange( column_index, max_value );
    }
    else if ( min_range + drange < min_value )
    {
        this->setMinRange( column_index, min_value );
        this->setMaxRange( column_index, min_value + range_width );
    }
    else
    {
        this->setMinRange( column_index, min_range + drange );
        this->setMaxRange( column_index, max_range + drange );
    }
}

/*===========================================================================*/
/**
 *  @brief  Resets range specified by column_index.
 *  @param  column_index [in] column index
 */
/*===========================================================================*/
void TableObject::resetRange( const size_t column_index )
{
    this->setMinRange( column_index, this->minValue(column_index) );
    this->setMaxRange( column_index, this->maxValue(column_index) );
}

/*===========================================================================*/
/**
 *  @brief  Resets ranges.
 */
/*===========================================================================*/
void TableObject::resetRange()
{
    const size_t ncolumns = this->numberOfColumns();
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        m_max_ranges[i] = this->maxValue(i);
        m_min_ranges[i] = this->minValue(i);
    }

    std::fill( m_inside_range_flags.begin(), m_inside_range_flags.end(), 1 );
}

template<> void TableObject::addColumn<kvs::Int8>( const kvs::ValueArray<kvs::Int8>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt8>( const kvs::ValueArray<kvs::UInt8>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int16>( const kvs::ValueArray<kvs::Int16>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt16>( const kvs::ValueArray<kvs::UInt16>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int32>( const kvs::ValueArray<kvs::Int32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt32>( const kvs::ValueArray<kvs::UInt32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int64>( const kvs::ValueArray<kvs::Int64>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt64>( const kvs::ValueArray<kvs::UInt64>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Real32>( const kvs::ValueArray<kvs::Real32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Real64>( const kvs::ValueArray<kvs::Real64>& array, const std::string& label );
template<> void TableObject::addColumn<std::string>( const kvs::ValueArray<std::string>& array, const std::string& label );

template<> void TableObject::addColumn<kvs::Int8>( const std::vector<kvs::Int8>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt8>( const std::vector<kvs::UInt8>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int16>( const std::vector<kvs::Int16>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt16>( const std::vector<kvs::UInt16>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int32>( const std::vector<kvs::Int32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt32>( const std::vector<kvs::UInt32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Int64>( const std::vector<kvs::Int64>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::UInt64>( const std::vector<kvs::UInt64>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Real32>( const std::vector<kvs::Real32>& array, const std::string& label );
template<> void TableObject::addColumn<kvs::Real64>( const std::vector<kvs::Real64>& array, const std::string& label );
template<> void TableObject::addColumn<std::string>( const std::vector<std::string>& array, const std::string& label );

template<> const kvs::Int8& TableObject::at<kvs::Int8>( const size_t row, const size_t column ) const;
template<> const kvs::UInt8& TableObject::at<kvs::UInt8>( const size_t row, const size_t column ) const;
template<> const kvs::Int16& TableObject::at<kvs::Int16>( const size_t row, const size_t column ) const;
template<> const kvs::UInt16& TableObject::at<kvs::UInt16>( const size_t row, const size_t column ) const;
template<> const kvs::Int32& TableObject::at<kvs::Int32>( const size_t row, const size_t column ) const;
template<> const kvs::UInt32& TableObject::at<kvs::UInt32>( const size_t row, const size_t column ) const;
template<> const kvs::Int64& TableObject::at<kvs::Int64>( const size_t row, const size_t column ) const;
template<> const kvs::UInt64& TableObject::at<kvs::UInt64>( const size_t row, const size_t column ) const;
template<> const kvs::Real32& TableObject::at<kvs::Real32>( const size_t row, const size_t column ) const;
template<> const kvs::Real64& TableObject::at<kvs::Real64>( const size_t row, const size_t column ) const;
template<> const std::string& TableObject::at<std::string>( const size_t row, const size_t column ) const;

} // end of namespace kvs
