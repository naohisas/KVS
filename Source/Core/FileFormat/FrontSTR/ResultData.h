/*****************************************************************************/
/**
 *  @file   ResultData.h
 *  @author Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResultData.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__FSTR__RESULT_DATA_H_INCLUDE
#define KVS__FSTR__RESULT_DATA_H_INCLUDE

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Indent>


namespace kvs
{

namespace fstr
{

/*===========================================================================*/
/**
 *  @brief  Result data class.
 */
/*===========================================================================*/
class ResultData
{
public:

    typedef kvs::ValueArray<kvs::Real32> Values;

protected:

    size_t m_nnodes; ///< number of nodes
    size_t m_ncells; ///< number of cells
    size_t m_ncomponents_per_node; ///< number of components per node
    size_t m_ncomponents_per_cell; ///< number of components per cell
    std::vector<size_t> m_veclens; ///< vector length of components
    std::vector<std::string> m_labels; ///< label of components
    std::vector<Values> m_values; ///< values of components

public:

    ResultData();

    size_t numberOfNodes() const;
    size_t numberOfCells() const;
    size_t numberOfComponentsPerNode() const;
    size_t numberOfComponentsPerCell() const;
    const std::vector<size_t>& veclens() const;
    const std::vector<std::string>& labels() const;
    const std::vector<Values>& values() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool readData( const std::string& filename );
    bool readDividedData( const std::string& filename );

private:

    bool read_nnodes_and_ncells( std::string& line, std::ifstream& ifs );
    bool read_veclens( std::string& line, std::ifstream& ifs );
    bool read_labels( std::string& line, std::ifstream& ifs );
};

} // end of namespace fstr

} // end of namespace kvs

#endif // KVS__FSTR__RESULT_DATA_H_INCLUDE
