/****************************************************************************/
/**
 *  @file   Tokenizer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  Delimit tokens.
 */
/*==========================================================================*/
class Tokenizer
{
private:
    std::string m_source; ///< source std::string
    std::string m_delimiter; ///< delimiter set
    std::string::size_type m_next; ///< next token position

public:
    Tokenizer();
    Tokenizer( const std::string& source, const std::string& delimiter );

    Tokenizer& operator = ( const std::string& source );

    bool isLast() const;
    std::string token();
};

} // end of namespace kvs
