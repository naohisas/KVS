/*****************************************************************************/
/**
 *  @file   Indent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>


namespace kvs
{

class Indent
{
private:
    size_t m_width; ///< indent width

public:
    explicit Indent( const size_t width = 0 ): m_width( width ) {}

    Indent nextIndent() const;
    friend std::ostream& operator << ( std::ostream& os, const kvs::Indent& indent );
};

} // end of namespace kvs
