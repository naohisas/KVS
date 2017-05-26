#pragma once
#include "Tag.h"
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

class FeatureTag : public Tag
{
private:
    std::string m_featureName; ///< name of feature
    float m_featureScale; ///< scaling ration of feature
    std::string m_featureUnit; ///< measurement unit of feature

public:
    FeatureTag();

    const std::string& featureName() const { return m_featureName; }
    float featureScale() const { return m_featureScale; }
    const std::string& featureUnit() const { return m_featureUnit; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
