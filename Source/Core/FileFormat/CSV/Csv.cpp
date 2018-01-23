/*****************************************************************************/
/**
 *  @file   Csv.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Csv.cpp 1346 2012-11-08 11:57:55Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "Csv.h"
#include <fstream>
#include <sstream>
#include <kvs/Message>
#include <kvs/File>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true, if the given file is CSV format
 */
/*===========================================================================*/
bool Csv::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "csv" || file.extension() == "CSV" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Csv calss.
 */
/*===========================================================================*/
Csv::Csv()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Csv class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
Csv::Csv( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of rows.
 *  @return number of rows
 */
/*===========================================================================*/
size_t Csv::numberOfRows() const
{
    return m_table.size();
}

/*===========================================================================*/
/**
 *  @brief  Returns the row.
 *  @param  index [in] index of the row
 *  @return row
 */
/*===========================================================================*/
const Csv::Row& Csv::row( const size_t index ) const
{
    return m_table.at(index);
}

/*===========================================================================*/
/**
 *  @brief  Returns the value.
 *  @param  i [in] row index
 *  @param  j [in] column index
 *  @return value
 */
/*===========================================================================*/
const std::string& Csv::value( const size_t i, const size_t j ) const
{
    return m_table.at(i).at(j);
}

/*===========================================================================*/
/**
 *  @brief  Adds a row.
 *  @param  row [in] row
 */
/*===========================================================================*/
void Csv::addRow( const Row& row )
{
    m_table.push_back( row );
}

/*===========================================================================*/
/**
 *  @brief  Sets a row.
 *  @param  index [in] row index
 *  @param  row [in] row
 */
/*===========================================================================*/
void Csv::setRow( const size_t index, const Row& row )
{
    m_table.at(index) = row;
}

/*===========================================================================*/
/**
 *  @brief  Sets a value.
 *  @param  i [in] row index
 *  @param  j [in] column index
 *  @param  value [in] value
 */
/*===========================================================================*/
void Csv::setValue( const size_t i, const size_t j, const std::string& value )
{
    m_table.at(i).at(j) = value;
}

/*===========================================================================*/
/**
 *  @brief  Output the information of CSV data.
 *  @param  os [in] output stream
 *  @param  indent [in] indent size (number of whitespaces)
 */
/*===========================================================================*/
void Csv::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of rows : " << m_table.size() << std::endl;
    os << indent << "First line : ";
    const Csv::Row& row = m_table.at(0);
    Csv::Row::const_iterator v = row.begin();
    os << *(v++); while ( v != row.end() ) os << ", " << *(v++);
}

/*===========================================================================*/
/**
 *  @brief  Read CSV data.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Csv::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ifstream ifs( filename.c_str() );
    if ( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    Row row;
    Item item;
    bool reading = false;

    while ( !ifs.eof() )
    {
        int c = ifs.get();
        if ( c == ',' )
        {
            if ( reading );
            else { row.push_back( item ); item.erase(); }
        }
        else if ( c == '"' )
        {
            if ( reading ) { reading = false; }
            else { reading = true; }
        }
        // Linefeed code: Windows CRLF(\r\n), Unix LF(\n), Mac CR(\r)
        else if ( c == '\n' || c == '\r' || c == '\0' )
        {
            if ( c == '\r' ) { c = ifs.peek(); if ( c == '\n' ) c = ifs.get(); }

            if ( reading ) { item.push_back( '\n' ); }
            else
            {
                row.push_back( item ); item.erase();
                m_table.push_back( row ); row.clear();
            }
        }
        else
        {
            item.push_back( static_cast<char>( c ) );
        }
    }

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write CSV data.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool Csv::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ofstream ofs( filename.c_str() );
    if ( !ofs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    std::vector<Csv::Row>::iterator row = m_table.begin();
    while ( row != m_table.end() )
    {
        Csv::Row::iterator c = row->begin();
        ofs << *(c++); while ( c != row->end() ) ofs << ", " << *(c++);
        ofs << std::endl;
        row++;
    }

    ofs.close();

    return true;
}

} // end of namespace kvs
