#pragma once
#include "Tag.h"
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

class PropertyTag : public Tag
{
private:
    std::string m_featureRef; ///< reference to the feature
    float m_featureVal; ///< value of the feature

public:
    PropertyTag();

    const std::string& featureRef() const { return m_featureRef; }
    float featureVal() const { return m_featureVal; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
