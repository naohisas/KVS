/*****************************************************************************/
/**
 *  @file   Csv.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Csv.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__CSV_H_INCLUDE
#define KVS__CSV_H_INCLUDE

#include <vector>
#include <string>
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  CSV (Comma Separated Value) class.
 */
/*===========================================================================*/
class Csv : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;
    typedef std::string Item;
    typedef std::vector<Item> Row;
    typedef std::vector<Row> Table;

protected:

    Table m_table; ///< row array

public:

    static bool CheckExtension( const std::string& filename );

public:

    Csv();
    Csv( const std::string& filename );

    size_t numberOfRows() const;
    const Row& row( const size_t index ) const;
    const std::string& value( const size_t i, const size_t j ) const;
    void addRow( const Row& row );
    void setRow( const size_t index, const Row& row );
    void setValue( const size_t i, const size_t j, const std::string& value );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

public:
    KVS_DEPRECATED( size_t nrows() const ) { return this->numberOfRows(); }
};

} // end of namespace kvs

#endif // KVS__CSV_H_INCLUDE
